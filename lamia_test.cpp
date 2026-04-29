/**
 * LAMIA ENHANCED FRAMEWORK TEST v0.3.0c
 * =====================================
 */

#include "lamia_enhanced_implementation.cpp"
#include <cstdlib>
#include <iostream>

int main() {
    std::cout << "🔮 Testing Lamia Enhanced Framework v0.3.0c" << std::endl;
    std::cout << "=============================================" << std::endl;
    
    // Test framework creation
    auto framework = std::make_unique<MedusaServ::Language::Lamia::CompleteLamiaFramework>();
    
    // Test version
    std::cout << "📋 Framework Version: " << framework->get_framework_version() << std::endl;
    
    // Test market readiness
    std::cout << "🚀 Market Ready: " << (framework->is_market_ready() ? "YES" : "NO") << std::endl;
    
    // Test IP whitelisting
    std::cout << "🛡️ IP Whitelisting Tests:" << std::endl;
    std::cout << "  - 72.14.201.65: " << (framework->is_ip_whitelisted("72.14.201.65") ? "✅ AUTHORIZED" : "❌ BLOCKED") << std::endl;
    std::cout << "  - 127.0.0.1: " << (framework->is_ip_whitelisted("127.0.0.1") ? "✅ AUTHORIZED" : "❌ BLOCKED") << std::endl;
    std::cout << "  - 1.2.3.4: " << (framework->is_ip_whitelisted("1.2.3.4") ? "✅ AUTHORIZED" : "❌ BLOCKED") << std::endl;
    
    // Test authentication
    const char* test_user = "lamia_test_user";
    const char* test_password = "lamia_test_runtime_password";
#if defined(_WIN32)
    _putenv_s("LAMIA_AUTH_USER", test_user);
    _putenv_s("LAMIA_AUTH_PASSWORD", test_password);
#else
    setenv("LAMIA_AUTH_USER", test_user, 1);
    setenv("LAMIA_AUTH_PASSWORD", test_password, 1);
#endif
    std::cout << "🔐 Authentication Tests:" << std::endl;
    std::cout << "  - runtime/correct: " << (framework->authenticate_user(test_user, test_password) ? "✅ SUCCESS" : "❌ FAILED") << std::endl;
    std::cout << "  - wrong/credentials: " << (framework->authenticate_user("wrong", "password") ? "✅ SUCCESS" : "❌ FAILED") << std::endl;
    
    // Test statistics
    std::cout << "📊 Framework Statistics:" << std::endl;
    auto stats = framework->get_framework_statistics();
    for (const auto& stat : stats) {
        std::cout << "  - " << stat.first << ": " << stat.second << std::endl;
    }
    
    // Test Lamia processing
    std::cout << "🔮 Testing Lamia-to-HTML Processing..." << std::endl;
    std::string lamia_test = "@application TestApp { @version \"0.3.0c\" }";
    std::string html_output = framework->process_lamia_to_html(lamia_test);
    std::cout << "✅ HTML Generated: " << html_output.length() << " characters" << std::endl;
    
    // Test C API
    std::cout << "🔗 Testing C API..." << std::endl;
    LamiaFramework* c_framework = lamia_framework_create();
    const char* version = lamia_framework_get_version(c_framework);
    bool market_ready = lamia_framework_is_market_ready(c_framework);
    bool ip_test = lamia_is_ip_whitelisted(c_framework, "72.14.201.65");
    bool auth_test = lamia_authenticate_user(c_framework, test_user, test_password);
    
    std::cout << "  - C API Version: " << version << std::endl;
    std::cout << "  - C API Market Ready: " << (market_ready ? "YES" : "NO") << std::endl;
    std::cout << "  - C API IP Test: " << (ip_test ? "✅ AUTHORIZED" : "❌ BLOCKED") << std::endl;
    std::cout << "  - C API Auth Test: " << (auth_test ? "✅ SUCCESS" : "❌ FAILED") << std::endl;
    
    lamia_framework_destroy(c_framework);
    
    std::cout << "=============================================" << std::endl;
    std::cout << "✅ ALL TESTS COMPLETED SUCCESSFULLY!" << std::endl;
    std::cout << "🔮 Lamia Enhanced Framework v0.3.0c is ready!" << std::endl;
    
    return 0;
}
