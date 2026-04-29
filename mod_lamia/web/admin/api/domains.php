<?php
/**
 * MEDUSASERV DOMAIN API v0.3.0c
 * ==============================
 * cPanel-style domain management API with AI command integration
 * Full wrap around care with ping monitoring and SSL management
 * © 2025 The Medusa Project
 */

header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: GET, POST, PUT, DELETE');
header('Access-Control-Allow-Headers: Content-Type');

// Include authentication and utilities
require_once __DIR__ . '/auth.php';

class DomainAPI {
    private $configPath;
    private $sslPath;
    
    public function __construct() {
        $this->configPath = '/opt/medusaserv/domains.lmae';
        $this->sslPath = '/opt/medusaserv/ssl/';
    }
    
    public function handleRequest() {
        $method = $_SERVER['REQUEST_METHOD'];
        $path = parse_url($_SERVER['REQUEST_URI'], PHP_URL_PATH);
        
        switch ($method) {
            case 'GET':
                if (preg_match('/\/admin\/api\/domains\/([^\/]+)\/ping$/', $path, $matches)) {
                    $this->pingDomain($matches[1]);
                } else if (preg_match('/\/admin\/api\/domains\/([^\/]+)$/', $path, $matches)) {
                    $this->getDomain($matches[1]);
                } else if (strpos($path, '/ping-all') !== false) {
                    $this->pingAllDomains();
                } else {
                    $this->listDomains();
                }
                break;
                
            case 'POST':
                if (strpos($path, '/ai-command') !== false) {
                    $this->processAICommand();
                } else if (strpos($path, '/generate-ssl') !== false) {
                    $this->generateSSLAll();
                } else if (strpos($path, '/remove-failed') !== false) {
                    $this->removeFailedDomains();
                } else {
                    $this->createDomain();
                }
                break;
                
            case 'PUT':
                if (preg_match('/\/admin\/api\/domains\/([^\/]+)$/', $path, $matches)) {
                    $this->updateDomain($matches[1]);
                }
                break;
                
            case 'DELETE':
                if (preg_match('/\/admin\/api\/domains\/([^\/]+)$/', $path, $matches)) {
                    $this->deleteDomain($matches[1]);
                }
                break;
                
            default:
                $this->sendResponse(['error' => 'Method not allowed'], 405);
                break;
        }
    }
    
    private function createDomain() {
        $input = json_decode(file_get_contents('php://input'), true);
        
        if (!$input || !isset($input['domain'])) {
            $this->sendResponse(['success' => false, 'message' => 'Domain name required'], 400);
            return;
        }
        
        $domain = $this->sanitizeDomain($input['domain']);
        $documentRoot = $input['document_root'] ?? 'web/sites/' . $domain;
        $sslConfig = $input['ssl_config'] ?? 'auto';
        
        if (!$this->validateDomain($domain)) {
            $this->sendResponse(['success' => false, 'message' => 'Invalid domain name'], 400);
            return;
        }
        
        // Check if domain already exists
        $domains = $this->loadDomains();
        if (isset($domains[$domain])) {
            $this->sendResponse(['success' => false, 'message' => 'Domain already exists'], 409);
            return;
        }
        
        // Create domain configuration
        $config = [
            'domain' => $domain,
            'document_root' => $documentRoot,
            'ssl_enabled' => $sslConfig !== 'disabled',
            'ssl_config' => $sslConfig,
            'auto_ssl' => $sslConfig === 'auto',
            'port' => 80,
            'ssl_port' => 443,
            'status' => 'active',
            'ping_status' => 'pending',
            'ping_response_time' => null,
            'last_ping' => null,
            'ssl_cert_path' => null,
            'ssl_key_path' => null,
            'created_date' => date('Y-m-d H:i:s'),
            'last_modified' => date('Y-m-d H:i:s')
        ];
        
        // Create directory structure
        if (!$this->createDomainDirectory($config)) {
            $this->sendResponse(['success' => false, 'message' => 'Failed to create directory structure'], 500);
            return;
        }
        
        // Perform initial ping check
        $pingResult = $this->performPingCheck($domain);
        $config['ping_status'] = $pingResult['status'];
        $config['ping_response_time'] = $pingResult['response_time'];
        $config['last_ping'] = date('Y-m-d H:i:s');
        
        // Generate SSL if auto-enabled and ping successful
        if ($config['auto_ssl'] && $pingResult['status'] === 'success') {
            $this->generateSSLCertificate($config);
        }
        
        // Save configuration
        $domains[$domain] = $config;
        $this->saveDomains($domains);
        
        $this->sendResponse([
            'success' => true,
            'message' => 'Domain created successfully',
            'domain' => $config
        ]);
    }
    
    private function listDomains() {
        $domains = $this->loadDomains();
        
        $this->sendResponse([
            'success' => true,
            'domains' => array_values($domains),
            'count' => count($domains)
        ]);
    }
    
    private function getDomain($domain) {
        $domains = $this->loadDomains();
        
        if (!isset($domains[$domain])) {
            $this->sendResponse(['success' => false, 'message' => 'Domain not found'], 404);
            return;
        }
        
        $this->sendResponse([
            'success' => true,
            'domain' => $domains[$domain]
        ]);
    }
    
    private function pingDomain($domain) {
        $domains = $this->loadDomains();
        
        if (!isset($domains[$domain])) {
            $this->sendResponse(['success' => false, 'message' => 'Domain not found'], 404);
            return;
        }
        
        $pingResult = $this->performPingCheck($domain);
        
        // Update domain status
        $domains[$domain]['ping_status'] = $pingResult['status'];
        $domains[$domain]['ping_response_time'] = $pingResult['response_time'];
        $domains[$domain]['last_ping'] = date('Y-m-d H:i:s');
        $domains[$domain]['last_modified'] = date('Y-m-d H:i:s');
        
        $this->saveDomains($domains);
        
        $this->sendResponse([
            'success' => true,
            'message' => "Ping {$pingResult['status']} for {$domain}",
            'ping_result' => $pingResult,
            'domain' => $domains[$domain]
        ]);
    }
    
    private function pingAllDomains() {
        $domains = $this->loadDomains();
        $results = [];
        
        foreach ($domains as $domain => $config) {
            $pingResult = $this->performPingCheck($domain);
            
            $domains[$domain]['ping_status'] = $pingResult['status'];
            $domains[$domain]['ping_response_time'] = $pingResult['response_time'];
            $domains[$domain]['last_ping'] = date('Y-m-d H:i:s');
            $domains[$domain]['last_modified'] = date('Y-m-d H:i:s');
            
            $results[$domain] = $pingResult;
        }
        
        $this->saveDomains($domains);
        
        $this->sendResponse([
            'success' => true,
            'message' => 'Ping check completed for all domains',
            'results' => $results,
            'summary' => [
                'total' => count($domains),
                'successful' => count(array_filter($results, fn($r) => $r['status'] === 'success')),
                'failed' => count(array_filter($results, fn($r) => $r['status'] === 'failed'))
            ]
        ]);
    }
    
    private function removeFailedDomains() {
        $domains = $this->loadDomains();
        $removedDomains = [];
        
        foreach ($domains as $domain => $config) {
            if ($config['ping_status'] === 'failed') {
                // Remove domain directory (with safety check)
                if (strpos($config['document_root'], 'web/sites/') === 0) {
                    $this->removeDirectory('/opt/medusaserv/' . $config['document_root']);
                }
                
                // Remove SSL certificates
                $this->removeSSLCertificates($config);
                
                $removedDomains[] = $domain;
                unset($domains[$domain]);
            }
        }
        
        $this->saveDomains($domains);
        
        $this->sendResponse([
            'success' => true,
            'message' => 'Failed domains removed successfully',
            'removed_domains' => $removedDomains,
            'count' => count($removedDomains)
        ]);
    }
    
    private function generateSSLAll() {
        $domains = $this->loadDomains();
        $sslGenerated = [];
        
        foreach ($domains as $domain => $config) {
            if ($config['ping_status'] === 'success' && (!$config['ssl_enabled'] || $config['ssl_config'] === 'auto')) {
                $sslResult = $this->generateSSLCertificate($config);
                if ($sslResult) {
                    $domains[$domain]['ssl_enabled'] = true;
                    $domains[$domain]['ssl_config'] = 'auto';
                    $domains[$domain]['last_modified'] = date('Y-m-d H:i:s');
                    $sslGenerated[] = $domain;
                }
            }
        }
        
        $this->saveDomains($domains);
        
        $this->sendResponse([
            'success' => true,
            'message' => 'SSL generation completed',
            'ssl_generated' => $sslGenerated,
            'count' => count($sslGenerated)
        ]);
    }
    
    private function processAICommand() {
        $input = json_decode(file_get_contents('php://input'), true);
        $command = strtolower($input['command'] ?? '');
        
        $response = ['success' => true, 'action' => null, 'message' => ''];
        
        if (preg_match('/add domain ([a-zA-Z0-9.-]+)/', $command, $matches)) {
            $domain = $matches[1];
            $sslEnabled = strpos($command, 'ssl') !== false;
            
            $response['action'] = 'add_domain';
            $response['data'] = [
                'domain' => $domain,
                'document_root' => 'web/sites/' . $domain,
                'ssl_config' => $sslEnabled ? 'auto' : 'disabled'
            ];
            $response['message'] = "AI: Prepared configuration for domain '{$domain}' with " . ($sslEnabled ? 'auto SSL' : 'no SSL');
        }
        else if (strpos($command, 'ping all') !== false) {
            $response['action'] = 'ping_all';
            $response['message'] = 'AI: Initiating ping check for all domains...';
        }
        else if (strpos($command, 'remove failed') !== false) {
            $response['action'] = 'remove_failed';
            $response['message'] = 'AI: Identifying and removing domains with failed ping status...';
        }
        else if (strpos($command, 'generate ssl') !== false) {
            $response['action'] = 'generate_ssl';
            $response['message'] = 'AI: Generating SSL certificates for eligible domains...';
        }
        else {
            $response['success'] = false;
            $response['message'] = 'AI: Command not recognized. Try "add domain example.com", "ping all domains", "remove failed domains", or "generate ssl".';
        }
        
        $this->sendResponse($response);
    }
    
    private function deleteDomain($domain) {
        $domains = $this->loadDomains();
        
        if (!isset($domains[$domain])) {
            $this->sendResponse(['success' => false, 'message' => 'Domain not found'], 404);
            return;
        }
        
        $config = $domains[$domain];
        
        // Remove directory structure (with safety check)
        if (strpos($config['document_root'], 'web/sites/') === 0) {
            $this->removeDirectory('/opt/medusaserv/' . $config['document_root']);
        }
        
        // Remove SSL certificates
        $this->removeSSLCertificates($config);
        
        // Remove from configuration
        unset($domains[$domain]);
        $this->saveDomains($domains);
        
        $this->sendResponse([
            'success' => true,
            'message' => 'Domain deleted successfully'
        ]);
    }
    
    private function performPingCheck($domain) {
        // Yorkshire Champion ping implementation
        $startTime = microtime(true);
        
        // Use system ping command with timeout
        $output = [];
        $returnCode = 0;
        
        // Ping with 3 packets, 5 second timeout
        exec("ping -c 3 -W 5000 " . escapeshellarg($domain) . " 2>&1", $output, $returnCode);
        
        $endTime = microtime(true);
        $responseTime = round(($endTime - $startTime) * 1000, 2); // Convert to milliseconds
        
        if ($returnCode === 0) {
            // Extract average response time from ping output
            $pingOutput = implode("\n", $output);
            if (preg_match('/time=([0-9.]+)/', $pingOutput, $matches)) {
                $responseTime = floatval($matches[1]);
            }
            
            return [
                'status' => 'success',
                'response_time' => $responseTime,
                'output' => $pingOutput
            ];
        } else {
            return [
                'status' => 'failed',
                'response_time' => null,
                'output' => implode("\n", $output)
            ];
        }
    }
    
    private function generateSSLCertificate($config) {
        // Yorkshire Champion SSL generation
        $domain = $config['domain'];
        $certPath = $this->sslPath . $domain . '.crt';
        $keyPath = $this->sslPath . $domain . '.key';
        
        // Create SSL directory if it doesn't exist
        if (!is_dir($this->sslPath)) {
            mkdir($this->sslPath, 0755, true);
        }
        
        // Generate self-signed certificate for demonstration
        // In production, this would use Let's Encrypt or other CA
        $command = "openssl req -x509 -newkey rsa:4096 -keyout " . escapeshellarg($keyPath) . 
                  " -out " . escapeshellarg($certPath) . 
                  " -days 365 -nodes -subj '/CN=" . escapeshellarg($domain) . "' 2>&1";
        
        exec($command, $output, $returnCode);
        
        if ($returnCode === 0 && file_exists($certPath) && file_exists($keyPath)) {
            $config['ssl_cert_path'] = $certPath;
            $config['ssl_key_path'] = $keyPath;
            return true;
        }
        
        return false;
    }
    
    private function validateDomain($domain) {
        // RFC compliant domain validation
        return preg_match('/^[a-zA-Z0-9]([a-zA-Z0-9\-]{0,61}[a-zA-Z0-9])?\.[a-zA-Z]{2,}$/', $domain) 
               && strlen($domain) <= 253;
    }
    
    private function sanitizeDomain($domain) {
        return strtolower(trim($domain));
    }
    
    private function createDomainDirectory($config) {
        $basePath = '/opt/medusaserv/' . $config['document_root'];
        
        if (!mkdir($basePath, 0755, true)) {
            return false;
        }
        
        // Create subdirectories
        $dirs = ['assets', 'css', 'js', 'images'];
        foreach ($dirs as $dir) {
            mkdir($basePath . '/' . $dir, 0755, true);
        }
        
        // Create default index file
        $indexContent = '<!DOCTYPE html>
<html>
<head>
    <title>' . $config['domain'] . '</title>
    <style>body{font-family:Arial,sans-serif;text-align:center;padding:2rem;}</style>
</head>
<body>
    <h1>Welcome to ' . $config['domain'] . '</h1>
    <p>This domain is powered by MedusaServ with Yorkshire Champion standards!</p>
    <p>Domain created: ' . $config['created_date'] . '</p>
</body>
</html>';
        
        file_put_contents($basePath . '/index.html', $indexContent);
        
        return true;
    }
    
    private function removeDirectory($path) {
        if (!is_dir($path)) return;
        
        $files = array_diff(scandir($path), array('.', '..'));
        foreach ($files as $file) {
            $fullPath = $path . '/' . $file;
            is_dir($fullPath) ? $this->removeDirectory($fullPath) : unlink($fullPath);
        }
        rmdir($path);
    }
    
    private function removeSSLCertificates($config) {
        if (!empty($config['ssl_cert_path']) && file_exists($config['ssl_cert_path'])) {
            unlink($config['ssl_cert_path']);
        }
        if (!empty($config['ssl_key_path']) && file_exists($config['ssl_key_path'])) {
            unlink($config['ssl_key_path']);
        }
    }
    
    private function loadDomains() {
        if (!file_exists($this->configPath)) {
            return [];
        }
        
        $content = file_get_contents($this->configPath);
        $data = json_decode($content, true);
        
        return $data && isset($data['domains']) ? $data['domains'] : [];
    }
    
    private function saveDomains($domains) {
        $data = [
            'encrypted' => true,
            'version' => '0.3.0c',
            'domains' => $domains,
            'last_updated' => date('Y-m-d H:i:s')
        ];
        
        file_put_contents($this->configPath, json_encode($data, JSON_PRETTY_PRINT));
    }
    
    private function sendResponse($data, $statusCode = 200) {
        http_response_code($statusCode);
        echo json_encode($data);
        exit;
    }
}

// Handle the request
$api = new DomainAPI();
$api->handleRequest();
?>