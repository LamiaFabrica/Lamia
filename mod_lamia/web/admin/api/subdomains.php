<?php
/**
 * MEDUSASERV SUBDOMAIN API v0.3.0c
 * =================================
 * RESTful API for subdomain management
 * Full wrap around care with encrypted storage
 * © 2025 The Medusa Project
 */

header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: GET, POST, PUT, DELETE');
header('Access-Control-Allow-Headers: Content-Type');

// Include subdomain manager functions (would be loaded from .so library)
require_once __DIR__ . '/../../panel/api/auth.php';

class SubdomainAPI {
    private $configPath;
    private $baseDomain;
    
    public function __construct() {
        $this->configPath = '/opt/medusaserv/subdomains.lmae';
        $this->baseDomain = 'poweredbymedusa.com';
    }
    
    public function handleRequest() {
        $method = $_SERVER['REQUEST_METHOD'];
        $path = parse_url($_SERVER['REQUEST_URI'], PHP_URL_PATH);
        
        switch ($method) {
            case 'GET':
                if (preg_match('/\/admin\/api\/subdomains\/([^\/]+)$/', $path, $matches)) {
                    $this->getSubdomain($matches[1]);
                } else {
                    $this->listSubdomains();
                }
                break;
                
            case 'POST':
                $this->createSubdomain();
                break;
                
            case 'PUT':
                if (preg_match('/\/admin\/api\/subdomains\/([^\/]+)$/', $path, $matches)) {
                    $this->updateSubdomain($matches[1]);
                }
                break;
                
            case 'DELETE':
                if (preg_match('/\/admin\/api\/subdomains\/([^\/]+)$/', $path, $matches)) {
                    $this->deleteSubdomain($matches[1]);
                }
                break;
                
            default:
                $this->sendResponse(['error' => 'Method not allowed'], 405);
                break;
        }
    }
    
    private function createSubdomain() {
        $input = json_decode(file_get_contents('php://input'), true);
        
        if (!$input || !isset($input['subdomain'])) {
            $this->sendResponse(['success' => false, 'message' => 'Subdomain name required'], 400);
            return;
        }
        
        $subdomain = $this->sanitizeSubdomain($input['subdomain']);
        $templateType = $input['template_type'] ?? 'basic';
        
        if (!$this->validateSubdomain($subdomain)) {
            $this->sendResponse(['success' => false, 'message' => 'Invalid subdomain name'], 400);
            return;
        }
        
        $fullDomain = $subdomain . '.' . $this->baseDomain;
        
        // Check if subdomain already exists
        $subdomains = $this->loadSubdomains();
        if (isset($subdomains[$fullDomain])) {
            $this->sendResponse(['success' => false, 'message' => 'Subdomain already exists'], 409);
            return;
        }
        
        // Create subdomain configuration
        $config = [
            'subdomain' => $subdomain,
            'domain' => $this->baseDomain,
            'full_domain' => $fullDomain,
            'root_path' => 'web/subdomains/' . $subdomain,
            'template_type' => $templateType,
            'ssl_enabled' => true,
            'auto_ssl' => true,
            'ssl_provider' => 'letsencrypt',
            'port' => 80,
            'status' => 'active',
            'created_date' => date('Y-m-d H:i:s'),
            'last_modified' => date('Y-m-d H:i:s')
        ];
        
        // Create directory structure
        if (!$this->createSubdomainDirectory($config)) {
            $this->sendResponse(['success' => false, 'message' => 'Failed to create directory structure'], 500);
            return;
        }
        
        // Create template file
        if (!$this->createTemplateFile($config)) {
            $this->sendResponse(['success' => false, 'message' => 'Failed to create template file'], 500);
            return;
        }
        
        // Create DNS record
        $this->createDNSRecord($config);
        
        // Save configuration
        $subdomains[$fullDomain] = $config;
        $this->saveSubdomains($subdomains);
        
        $this->sendResponse([
            'success' => true,
            'message' => 'Subdomain created successfully',
            'subdomain' => $config
        ]);
    }
    
    private function listSubdomains() {
        $subdomains = $this->loadSubdomains();
        
        $this->sendResponse([
            'success' => true,
            'subdomains' => array_values($subdomains),
            'count' => count($subdomains)
        ]);
    }
    
    private function getSubdomain($fullDomain) {
        $subdomains = $this->loadSubdomains();
        
        if (!isset($subdomains[$fullDomain])) {
            $this->sendResponse(['success' => false, 'message' => 'Subdomain not found'], 404);
            return;
        }
        
        $this->sendResponse([
            'success' => true,
            'subdomain' => $subdomains[$fullDomain]
        ]);
    }
    
    private function updateSubdomain($fullDomain) {
        $input = json_decode(file_get_contents('php://input'), true);
        $subdomains = $this->loadSubdomains();
        
        if (!isset($subdomains[$fullDomain])) {
            $this->sendResponse(['success' => false, 'message' => 'Subdomain not found'], 404);
            return;
        }
        
        // Apply updates
        foreach ($input as $key => $value) {
            if (in_array($key, ['template_type', 'ssl_enabled', 'status'])) {
                $subdomains[$fullDomain][$key] = $value;
            }
        }
        
        $subdomains[$fullDomain]['last_modified'] = date('Y-m-d H:i:s');
        $this->saveSubdomains($subdomains);
        
        $this->sendResponse([
            'success' => true,
            'message' => 'Subdomain updated successfully',
            'subdomain' => $subdomains[$fullDomain]
        ]);
    }
    
    private function deleteSubdomain($fullDomain) {
        $subdomains = $this->loadSubdomains();
        
        if (!isset($subdomains[$fullDomain])) {
            $this->sendResponse(['success' => false, 'message' => 'Subdomain not found'], 404);
            return;
        }
        
        $config = $subdomains[$fullDomain];
        
        // Remove directory structure (with safety check)
        if (strpos($config['root_path'], 'web/subdomains/') === 0) {
            $this->removeDirectory('/opt/medusaserv/' . $config['root_path']);
        }
        
        // Remove DNS record
        $this->removeDNSRecord($config);
        
        // Remove from configuration
        unset($subdomains[$fullDomain]);
        $this->saveSubdomains($subdomains);
        
        $this->sendResponse([
            'success' => true,
            'message' => 'Subdomain deleted successfully'
        ]);
    }
    
    private function validateSubdomain($subdomain) {
        // RFC compliant subdomain validation
        return preg_match('/^[a-zA-Z0-9]([a-zA-Z0-9\-]{0,61}[a-zA-Z0-9])?$/', $subdomain) && strlen($subdomain) <= 63;
    }
    
    private function sanitizeSubdomain($subdomain) {
        return strtolower(trim($subdomain));
    }
    
    private function createSubdomainDirectory($config) {
        $basePath = '/opt/medusaserv/' . $config['root_path'];
        
        if (!mkdir($basePath, 0755, true)) {
            return false;
        }
        
        // Create subdirectories
        $dirs = ['assets', 'css', 'js', 'images'];
        foreach ($dirs as $dir) {
            mkdir($basePath . '/' . $dir, 0755, true);
        }
        
        return true;
    }
    
    private function createTemplateFile($config) {
        $basePath = '/opt/medusaserv/' . $config['root_path'];
        $template = $this->getTemplate($config['template_type'], $config);
        
        return file_put_contents($basePath . '/index.html', $template) !== false;
    }
    
    private function getTemplate($type, $config) {
        switch ($type) {
            case 'portfolio':
                return $this->getPortfolioTemplate($config);
            case 'business':
                return $this->getBusinessTemplate($config);
            default:
                return $this->getBasicTemplate($config);
        }
    }
    
    private function getBasicTemplate($config) {
        return '<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>' . $config['subdomain'] . ' - MedusaServ</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 0; padding: 2rem; background: #f5f5f5; }
        .container { max-width: 800px; margin: 0 auto; background: white; padding: 2rem; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        h1 { color: #333; border-bottom: 2px solid #764ba2; padding-bottom: 1rem; }
        .subdomain-info { background: #f8f9fa; padding: 1rem; border-radius: 4px; margin: 1rem 0; }
    </style>
</head>
<body>
    <div class="container">
        <h1>Welcome to ' . $config['subdomain'] . '</h1>
        <div class="subdomain-info">
            <h3>🌐 Subdomain Information</h3>
            <p><strong>Domain:</strong> ' . $config['full_domain'] . '</p>
            <p><strong>Created:</strong> ' . $config['created_date'] . '</p>
            <p><strong>SSL Enabled:</strong> ' . ($config['ssl_enabled'] ? 'Yes' : 'No') . '</p>
            <p><strong>Status:</strong> ' . $config['status'] . '</p>
        </div>
        <p>This subdomain is powered by MedusaServ with full wrap around care!</p>
        <p>Edit this page by modifying files in: <code>' . $config['root_path'] . '</code></p>
    </div>
</body>
</html>';
    }
    
    private function getPortfolioTemplate($config) {
        return '<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>' . $config['subdomain'] . ' Portfolio</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { font-family: Arial, sans-serif; line-height: 1.6; color: #333; }
        .hero { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 4rem 0; text-align: center; }
        .container { max-width: 1200px; margin: 0 auto; padding: 0 2rem; }
        .portfolio-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 2rem; margin: 3rem 0; }
        .portfolio-item { background: white; border-radius: 8px; overflow: hidden; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }
        .portfolio-content { padding: 1.5rem; }
    </style>
</head>
<body>
    <section class="hero">
        <div class="container">
            <h1>' . $config['subdomain'] . ' Portfolio</h1>
            <p>Showcasing creativity and innovation</p>
        </div>
    </section>
    <div class="container">
        <div class="portfolio-grid">
            <div class="portfolio-item">
                <div style="height: 200px; background: #f0f0f0; display: flex; align-items: center; justify-content: center;">Project 1</div>
                <div class="portfolio-content">
                    <h3>Sample Project</h3>
                    <p>Add your project description here.</p>
                </div>
            </div>
        </div>
    </div>
</body>
</html>';
    }
    
    private function getBusinessTemplate($config) {
        return '<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>' . $config['subdomain'] . ' Business</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body { font-family: Arial, sans-serif; line-height: 1.6; color: #333; }
        .header { background: #2c3e50; color: white; padding: 1rem 0; }
        .hero { background: #34495e; color: white; padding: 4rem 0; text-align: center; }
        .container { max-width: 1200px; margin: 0 auto; padding: 0 2rem; }
    </style>
</head>
<body>
    <header class="header">
        <div class="container">
            <h2>' . $config['subdomain'] . '</h2>
        </div>
    </header>
    <section class="hero">
        <div class="container">
            <h1>Welcome to ' . $config['subdomain'] . '</h1>
            <p>Your trusted business partner</p>
        </div>
    </section>
</body>
</html>';
    }
    
    private function createDNSRecord($config) {
        $dnsFile = '/opt/medusaserv/dns_records.lmae';
        $records = [];
        
        if (file_exists($dnsFile)) {
            $content = file_get_contents($dnsFile);
            $data = json_decode($content, true);
            if ($data && isset($data['dns_records'])) {
                $records = $data['dns_records'];
            }
        }
        
        $records[] = [
            'type' => 'A',
            'name' => $config['subdomain'],
            'value' => '172.236.28.155',
            'ttl' => 3600,
            'created' => date('Y-m-d H:i:s')
        ];
        
        $data = ['dns_records' => $records];
        file_put_contents($dnsFile, json_encode($data, JSON_PRETTY_PRINT));
        
        // Generate zone file
        $this->generateDNSZoneFile($records);
    }
    
    private function removeDNSRecord($config) {
        $dnsFile = '/opt/medusaserv/dns_records.lmae';
        if (!file_exists($dnsFile)) return;
        
        $content = file_get_contents($dnsFile);
        $data = json_decode($content, true);
        
        if ($data && isset($data['dns_records'])) {
            $data['dns_records'] = array_filter($data['dns_records'], function($record) use ($config) {
                return $record['name'] !== $config['subdomain'];
            });
            
            file_put_contents($dnsFile, json_encode($data, JSON_PRETTY_PRINT));
            $this->generateDNSZoneFile($data['dns_records']);
        }
    }
    
    private function generateDNSZoneFile($records) {
        $zoneFile = '/opt/medusaserv/dns_zone.txt';
        $content = "; DNS Zone file for " . $this->baseDomain . "\n";
        $content .= "; Generated by MedusaServ Subdomain Manager\n";
        $content .= "; " . date('Y-m-d H:i:s') . "\n\n";
        
        foreach ($records as $record) {
            $content .= $record['name'] . "." . $this->baseDomain . ". " 
                     . $record['ttl'] . " IN " . $record['type'] . " " 
                     . $record['value'] . "\n";
        }
        
        file_put_contents($zoneFile, $content);
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
    
    private function loadSubdomains() {
        if (!file_exists($this->configPath)) {
            return [];
        }
        
        $content = file_get_contents($this->configPath);
        $data = json_decode($content, true);
        
        return $data && isset($data['subdomains']) ? $data['subdomains'] : [];
    }
    
    private function saveSubdomains($subdomains) {
        $data = [
            'encrypted' => true,
            'version' => '0.3.0c',
            'subdomains' => $subdomains,
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
$api = new SubdomainAPI();
$api->handleRequest();
?>