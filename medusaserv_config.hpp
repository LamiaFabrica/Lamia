/*
 * MedusaServ Master Configuration File
 * 
 * MIT LICENSE WITH MEDUSASERV RESTRICTIONS
 * © 2025 D Hargreaves AKA Roylepython | All Rights Reserved
 * 
 * This software is protected under Yorkshire Champion Standards
 * Standing Against Disability Discrimination
 * 
 * PROHIBITED ENTITIES: Kickstarter, WebHostingTalk, and any discriminatory entities
 * See LICENSE.md for full terms and conditions
 */

#ifndef MEDUSASERV_CONFIG_HPP
#define MEDUSASERV_CONFIG_HPP

#include <string>
#include <map>
#include <vector>

namespace MedusaServ {
namespace Config {

/**
 * Weinberg's Law: "If builders built buildings the way programmers wrote programs, 
 * then the first woodpecker that came along would destroy civilization."
 * - Gerald Weinberg, 1978
 * 
 * This configuration system follows Yorkshire Champion Standards to ensure
 * robust, maintainable, and accessible software architecture.
 */

// Core System Configuration
namespace Core {
    constexpr const char* VERSION = "0.3.0c";
    constexpr const char* CODENAME = "Yorkshire Champion";
    constexpr const char* BUILD_STANDARD = "Yorkshire Champion Standards";
    
    // Performance Configuration
    constexpr int PFDB_WORKER_THREADS = 8;
    constexpr double TOURNAMENT_ENGINE_MULTIPLIER = 6.0; // 600% improvement
    constexpr double YORKSHIRE_CHAMPION_MULTIPLIER = 15.0; // 15x performance
    constexpr double TARGET_RESPONSE_TIME_MS = 0.42;
    
    // Server Configuration
    constexpr int DEFAULT_PORT = 8080;
    constexpr int MAX_CONNECTIONS = 10000;
    constexpr int CONNECTION_TIMEOUT_SECONDS = 30;
    constexpr bool ENABLE_COMPRESSION = true;
}

// Membership Tier Configuration
namespace Membership {
    // Tier Pricing (in GBP) - Fair business-friendly model
    constexpr double INITIATE_PRICE = 5.0;         // With ads
    constexpr double ARCHITECT_PRICE = 25.0;       // Fair £25 ad-free threshold
    constexpr double GORGON_PRICE = 50.0;          // Premium business
    constexpr double GODMODE_CHAMPION_PRICE = 100.0; // Enterprise
    
    // Pioneer System
    constexpr bool ENABLE_PIONEERS_PAGE = true;
    constexpr const char* PIONEERS_PAGE_TITLE = "Official Pioneers - Foundational Supporters";
    constexpr const char* PIONEERS_DESCRIPTION = 
        "These visionary supporters believed in MedusaServ from the beginning, "
        "providing the foundational support that made revolutionary web server technology possible.";
}

// Advertising System Configuration
namespace Advertising {
    constexpr bool ENABLE_ADVERTISING = true;
    constexpr bool UNMOVABLE_ADS = true;
    constexpr bool UNSKIPPABLE_ADS = true;
    
    // Ad Display Times
    constexpr int MINIMUM_VIEW_TIME_SECONDS = 15;
    constexpr int INTERSTITIAL_VIEW_TIME_SECONDS = 30;
    constexpr int BANNER_REFRESH_SECONDS = 60;
    
    // Revenue Configuration
    constexpr double REVENUE_PER_VIEW = 0.02; // £0.02 per ad view
    constexpr double REVENUE_PER_CLICK = 0.50; // £0.50 per ad click
    
    // Ad-Free Roles (always exempt from ads)
    const std::vector<std::string> AD_FREE_ROLES = {
        "STAFF", "DEVELOPER", "ADMIN"
    };
}

// Security Configuration
namespace Security {
    constexpr bool ENABLE_ICEWALL_INTEGRATION = true;
    constexpr bool ENABLE_PQC_ENCRYPTION = true; // Post-Quantum Cryptography
    constexpr bool ENABLE_QUANTUM_RESISTANT = true;
    
    // Anti-Circumvention
    constexpr bool ENABLE_ANTI_ADBLOCK = true;
    constexpr int MAX_BYPASS_ATTEMPTS = 3;
    constexpr bool LOG_BYPASS_ATTEMPTS = true;
}

// Accessibility Configuration
namespace Accessibility {
    constexpr bool WCAG_21_COMPLIANCE = true;
    constexpr bool SCREEN_READER_OPTIMIZED = true;
    constexpr bool KEYBOARD_NAVIGATION_SUPPORT = true;
    constexpr bool COGNITIVE_ACCESSIBILITY = true;
    constexpr bool MULTI_INPUT_SUPPORT = true;
    constexpr bool REAL_TIME_MONITORING = true;
    
    // Font and Display
    constexpr int MINIMUM_FONT_SIZE = 12;
    constexpr double MINIMUM_CONTRAST_RATIO = 4.5;
    constexpr bool SUPPORT_HIGH_CONTRAST = true;
    constexpr bool SUPPORT_LARGE_TEXT = true;
}

// Build System Configuration
namespace Build {
    constexpr bool ENABLE_LIGHTSPEED_OPTIMIZATION = true;
    constexpr bool ENABLE_DEVMODE_EFFICIENCY = true;
    constexpr bool ENABLE_HOT_RELOADING = true;
    constexpr bool ENABLE_LIVE_UPDATES = true;
    
    // Optimization Levels
    constexpr int OPTIMIZATION_LEVEL = 2; // Perfect balance
    constexpr bool USE_SELECTIVE_OBFUSCATION = true;
    constexpr bool ENABLE_HARDWARE_CRYPTO = true;
    constexpr bool USE_AGGRESSIVE_CACHING = true;
}

// Rendering System Configuration  
namespace Rendering {
    constexpr bool ENABLE_SSR_PLUS = true;
    constexpr bool ENABLE_PSR_PLUS = true;
    constexpr bool ENABLE_PROGRESSIVE_STREAMING = true;
    constexpr bool ENABLE_AI_OPTIMIZATION = true;
    constexpr bool ENABLE_SECURITY_INTEGRATION = true;
    
    // Quality Settings
    constexpr int RENDERING_QUALITY_LEVEL = 3; // High quality
    constexpr bool ENABLE_COMPRESSION = true;
    constexpr bool ENABLE_MINIFICATION = true;
}

// Legal and Compliance
namespace Legal {
    constexpr const char* COPYRIGHT_NOTICE = "© 2025 D Hargreaves AKA Roylepython | All Rights Reserved";
    constexpr const char* LICENSE_TYPE = "MIT LICENSE WITH MEDUSASERV RESTRICTIONS";
    
    // Prohibited Entities
    const std::vector<std::string> PROHIBITED_ENTITIES = {
        "kickstarter.com",
        "webhostingtalk.com",
        "wht.com"
    };
    
    // Attribution Requirements
    constexpr const char* REQUIRED_FOOTER_TEXT = "Powered by LamiaFabrica - MedusaServ Technology";
    constexpr const char* REQUIRED_FOOTER_LINK = "https://lamiaFabrica.project";
    constexpr bool FOOTER_ATTRIBUTION_REQUIRED = true;
    constexpr double COPYRIGHT_VIOLATION_PENALTY = 150000.0; // £150,000 statutory damages
}

// Discrimination Protection
namespace AntiDiscrimination {
    constexpr bool ZERO_TOLERANCE_POLICY = true;
    constexpr bool ACTIVE_SUPPORT_DISABLED = true;
    constexpr bool CELEBRATE_NEURODIVERSITY = true;
    constexpr bool PROTECT_VULNERABLE_MEMBERS = true;
    
    // Protected Characteristics
    const std::vector<std::string> PROTECTED_CHARACTERISTICS = {
        "Physical disabilities",
        "Mental health conditions", 
        "Neurodivergent conditions (Autism, ADHD, Dyslexia, etc.)",
        "Chronic illnesses",
        "Sensory impairments",
        "Cognitive differences",
        "Any condition covered under disability legislation"
    };
    
    constexpr const char* DISCRIMINATION_STATEMENT = 
        "This software stands in solidarity with disabled developers, users, and contributors "
        "worldwide. We will not tolerate discrimination in any form.";
}

// Inspirational Quotes and Laws
namespace Wisdom {
    constexpr const char* WEINBERG_LAW = 
        "If builders built buildings the way programmers wrote programs, "
        "then the first woodpecker that came along would destroy civilization.";
    constexpr const char* WEINBERG_ATTRIBUTION = "Gerald Weinberg, 1978";
    
    constexpr const char* PROJECT_MOTTO = 
        "Technology should empower everyone, not discriminate against anyone.";
        
    constexpr const char* YORKSHIRE_CHAMPION_CREED = 
        "Excellence in engineering, Integrity in conduct, Protection of vulnerable users, "
        "Standing up for what's right, Never compromising on principles.";
}

// System Messages
namespace Messages {
    constexpr const char* WELCOME_MESSAGE = 
        "Welcome to MedusaServ - Revolutionary web server technology built with "
        "AI assistance as reasonable adjustment for disability. Standing against discrimination.";
        
    constexpr const char* AD_SUPPORT_MESSAGE = 
        "Ads support disabled developer & revolutionary technology development. "
        "Upgrade membership to remove ads and get premium features.";
        
    constexpr const char* ACCESSIBILITY_MESSAGE = 
        "MedusaServ is built with accessibility first - 100% compliant by design, not afterthought.";
}

} // namespace Config
} // namespace MedusaServ

#endif // MEDUSASERV_CONFIG_HPP