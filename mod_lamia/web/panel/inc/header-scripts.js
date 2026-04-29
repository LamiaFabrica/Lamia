/**
 * Revolutionary Enterprise Header Scripts
 * @fileoverview Manages the MedusaServ enterprise header with comprehensive security, performance, and accessibility features
 * @version 0.3.0c-ULTIMATE
 * @author MedusaServ Development Team
 * @license MIT
 */

/**
 * @class MedusaHeaderManager
 * @description Comprehensive header management system with security, performance, and accessibility features
 */
class MedusaHeaderManager {
    constructor() {
        this.isUserMenuOpen = false;
        this.isNotificationMenuOpen = false;
        this.updateAvailable = true; // Set to true for demo
        this.csrfToken = this.getCsrfToken();
        this.eventListeners = [];
        this.intervals = [];
        this.currentUser = {
            name: 'Administrator',
            email: 'medusa@localhost',
            role: 'System Admin',
            avatar: 'data:image/svg+xml,<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100"><circle cx="50" cy="35" r="20" fill="%23fff"></circle><path d="M20 80 C20 60, 35 50, 50 50 S80 60, 80 80" fill="%23fff"></path></svg>',
            isOnline: true,
            badges: ['Admin', 'Yorkshire Champion']
        };
        
        this.init();
    }

    /**
     * Retrieves CSRF token for secure API requests
     * @returns {string|null} CSRF token or null if not found
     */
    getCsrfToken() {
        const metaTag = document.querySelector('meta[name="csrf-token"]');
        if (metaTag) {
            return metaTag.getAttribute('content');
        }
        
        // Fallback: try to get from cookie
        const cookieMatch = document.cookie.match(/csrf_token=([^;]*)/);
        if (cookieMatch) {
            return cookieMatch[1];
        }
        
        console.warn('CSRF token not found. Security features may be disabled.');
        return null;
    }

    // HTML Sanitization
    sanitizeHTML(str) {
        if (typeof str !== 'string') return '';
        const temp = document.createElement('div');
        temp.textContent = str;
        return temp.innerHTML;
    }

    // Input Validation
    validateInput(input, type = 'text', maxLength = 1000) {
        if (typeof input !== 'string') {
            throw new Error('Input must be a string');
        }
        
        if (input.length > maxLength) {
            throw new Error(`Input too long (max ${maxLength} characters)`);
        }
        
        // Basic XSS pattern detection
        const xssPatterns = [
            /<script[^>]*>.*?<\/script>/gi,
            /javascript:/gi,
            /on\w+\s*=/gi,
            /<iframe[^>]*>.*?<\/iframe>/gi
        ];
        
        for (const pattern of xssPatterns) {
            if (pattern.test(input)) {
                throw new Error('Invalid input detected');
            }
        }
        
        return input.trim();
    }

    // Error Logging
    logError(message, error) {
        console.error(`[MedusaHeader Error] ${message}:`, error);
        
        // Send to server if CSRF token available
        if (this.csrfToken) {
            fetch('/api/log-error', {
                method: 'POST',
                headers: {
                    'X-CSRF-TOKEN': this.csrfToken,
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    component: 'MedusaHeader',
                    message: message,
                    error: error.toString(),
                    timestamp: new Date().toISOString()
                })
            }).catch(err => console.error('Failed to log error to server:', err));
        }
    }

    // Event Listener Management
    addEventListenerWithCleanup(element, event, handler) {
        element.addEventListener(event, handler);
        this.eventListeners.push({ element, event, handler });
    }

    // Cleanup method
    destroy() {
        // Clean up intervals
        this.intervals.forEach(interval => clearInterval(interval));
        
        // Remove event listeners
        this.eventListeners.forEach(({ element, event, handler }) => {
            element.removeEventListener(event, handler);
        });
        
        // Clear arrays
        this.intervals = [];
        this.eventListeners = [];
    }

    init() {
        console.log('🔮 Initializing MedusaServ Header Manager...');
        this.cacheDOMElements();
        this.setupEventListeners();
        this.initializeSearch();
        this.showUpdateWarning();
        this.startStatusUpdates();
        console.log('✨ Header Manager Online!');
    }

    // DOM Element Caching for Performance
    cacheDOMElements() {
        this.domCache = {
            userDropdown: document.getElementById('userDropdown'),
            notificationDropdown: document.getElementById('notificationDropdown'),
            globalSearch: document.getElementById('globalSearch'),
            searchSuggestions: document.getElementById('searchSuggestions'),
            updateWarning: document.getElementById('updateWarning'),
            notificationBell: document.querySelector('.notification-bell'),
            userAvatar: document.querySelector('.user-avatar'),
            userMenuToggle: document.querySelector('.user-menu-toggle'),
            searchContainer: document.querySelector('.search-container'),
            quickAccessItems: document.querySelectorAll('.quick-search-item'),
            notificationCenter: document.querySelector('.notification-center'),
            headerActions: document.querySelector('.header-actions')
        };
    }

    // Optimized DOM queries using cache
    getCachedElement(key) {
        if (!this.domCache[key]) {
            console.warn(`Cached element '${key}' not found. Performing fresh query.`);
            this.refreshCacheElement(key);
        }
        return this.domCache[key];
    }

    refreshCacheElement(key) {
        const selectors = {
            userDropdown: '#userDropdown',
            notificationDropdown: '#notificationDropdown',
            globalSearch: '#globalSearch',
            searchSuggestions: '#searchSuggestions',
            updateWarning: '#updateWarning',
            notificationBell: '.notification-bell',
            userAvatar: '.user-avatar',
            userMenuToggle: '.user-menu-toggle',
            searchContainer: '.search-container',
            quickAccessItems: '.quick-search-item',
            notificationCenter: '.notification-center',
            headerActions: '.header-actions'
        };

        if (selectors[key]) {
            this.domCache[key] = key.includes('All') ? 
                document.querySelectorAll(selectors[key]) : 
                document.querySelector(selectors[key]);
        }
    }

    setupEventListeners() {
        // User menu toggle
        document.addEventListener('click', (e) => {
            if (e.target.closest('.user-avatar-container') || e.target.closest('.user-menu-toggle')) {
                this.toggleUserMenu();
            } else if (!e.target.closest('.user-dropdown')) {
                this.closeUserMenu();
            }
        });

        // Notification toggle
        const notificationBtn = document.querySelector('.notification-center .action-btn');
        if (notificationBtn) {
            notificationBtn.addEventListener('click', (e) => {
                e.stopPropagation();
                this.toggleNotifications();
            });
        }

        // Search functionality
        const searchInput = document.getElementById('globalSearch');
        if (searchInput) {
            searchInput.addEventListener('input', (e) => this.handleSearch(e.target.value));
            searchInput.addEventListener('focus', () => this.showQuickAccess());
            searchInput.addEventListener('blur', () => setTimeout(() => this.hideQuickAccess(), 200));
        }

        // Keyboard shortcuts
        document.addEventListener('keydown', (e) => {
            if (e.ctrlKey && e.key === 'k') {
                e.preventDefault();
                this.focusSearch();
            }
            if (e.key === 'Escape') {
                this.closeAllMenus();
            }
        });

        // Close menus when clicking outside
        document.addEventListener('click', (e) => {
            if (!e.target.closest('.notification-center')) {
                this.closeNotificationMenu();
            }
        });
    }

    // User Menu Management
    toggleUserMenu() {
        const dropdown = this.getCachedElement('userDropdown');
        const toggle = dropdown?.querySelector('i');
        
        if (dropdown) {
            this.isUserMenuOpen = !this.isUserMenuOpen;
            dropdown.classList.toggle('show', this.isUserMenuOpen);
            
            if (toggle) {
                toggle.style.transform = this.isUserMenuOpen ? 'rotate(180deg)' : 'rotate(0deg)';
            }
            
            // Close notification menu if open
            if (this.isUserMenuOpen) {
                this.closeNotificationMenu();
            }
        }
    }

    closeUserMenu() {
        const dropdown = this.getCachedElement('userDropdown');
        const toggle = dropdown?.querySelector('i');
        
        if (dropdown && this.isUserMenuOpen) {
            this.isUserMenuOpen = false;
            dropdown.classList.remove('show');
            
            if (toggle) {
                toggle.style.transform = 'rotate(0deg)';
            }
        }
    }

    // Notification Management
    toggleNotifications() {
        const dropdown = document.getElementById('notificationDropdown');
        
        if (dropdown) {
            this.isNotificationMenuOpen = !this.isNotificationMenuOpen;
            dropdown.classList.toggle('show', this.isNotificationMenuOpen);
            
            // Close user menu if open
            if (this.isNotificationMenuOpen) {
                this.closeUserMenu();
            }
        }
    }

    closeNotificationMenu() {
        const dropdown = document.getElementById('notificationDropdown');
        
        if (dropdown && this.isNotificationMenuOpen) {
            this.isNotificationMenuOpen = false;
            dropdown.classList.remove('show');
        }
    }

    markAllRead() {
        const notifications = document.querySelectorAll('.notification-item.unread');
        notifications.forEach(notification => {
            notification.classList.remove('unread');
        });
        
        // Update notification count
        const countElement = document.getElementById('notificationCount');
        if (countElement) {
            countElement.textContent = '0';
            countElement.style.display = 'none';
        }
        
        console.log('📫 All notifications marked as read');
    }

    // Search Management
    initializeSearch() {
        this.searchSuggestions = [
            { term: 'services', category: 'System', icon: 'fas fa-cogs' },
            { term: 'security logs', category: 'Security', icon: 'fas fa-shield-alt' },
            { term: 'user management', category: 'Admin', icon: 'fas fa-users' },
            { term: 'performance metrics', category: 'Monitoring', icon: 'fas fa-chart-line' },
            { term: 'backup status', category: 'Backup', icon: 'fas fa-archive' },
            { term: 'ssl certificates', category: 'Security', icon: 'fas fa-certificate' }
        ];
    }

    handleSearch(query) {
        if (query.length < 2) {
            this.hideSuggestions();
            return;
        }

        const filteredSuggestions = this.searchSuggestions.filter(suggestion =>
            suggestion.term.toLowerCase().includes(query.toLowerCase()) ||
            suggestion.category.toLowerCase().includes(query.toLowerCase())
        );

        this.showSuggestions(filteredSuggestions, query);
    }

    showSuggestions(suggestions, query) {
        const suggestionsContainer = document.getElementById('searchSuggestions');
        if (!suggestionsContainer) return;

        try {
            // Validate and sanitize query
            const sanitizedQuery = this.sanitizeHTML(this.validateInput(query, 'text', 100));
            
            // Clear container safely
            suggestionsContainer.innerHTML = '';

            if (suggestions.length === 0) {
                const noResultsDiv = document.createElement('div');
                noResultsDiv.className = 'no-suggestions';
                
                const icon = document.createElement('i');
                icon.className = 'fas fa-search';
                
                const span = document.createElement('span');
                span.textContent = `No results found for "${sanitizedQuery}"`;
                
                noResultsDiv.appendChild(icon);
                noResultsDiv.appendChild(span);
                suggestionsContainer.appendChild(noResultsDiv);
            } else {
                // Create suggestions safely using DOM methods
                suggestions.forEach(suggestion => {
                    try {
                        const suggestionDiv = document.createElement('div');
                        suggestionDiv.className = 'suggestion-item';
                        
                        // Sanitize all suggestion data
                        const sanitizedTerm = this.sanitizeHTML(this.validateInput(suggestion.term, 'text', 200));
                        const sanitizedCategory = this.sanitizeHTML(this.validateInput(suggestion.category, 'text', 100));
                        const sanitizedIcon = this.sanitizeHTML(this.validateInput(suggestion.icon, 'text', 50));
                        
                        const icon = document.createElement('i');
                        icon.className = sanitizedIcon;
                        
                        const termSpan = document.createElement('span');
                        termSpan.className = 'suggestion-term';
                        termSpan.innerHTML = this.highlightMatch(sanitizedTerm, sanitizedQuery);
                        
                        const categorySpan = document.createElement('span');
                        categorySpan.className = 'suggestion-category';
                        categorySpan.textContent = sanitizedCategory;
                        
                        // Use event listener instead of onclick
                        suggestionDiv.addEventListener('click', () => {
                            this.selectSuggestion(sanitizedTerm);
                        });
                        
                        suggestionDiv.appendChild(icon);
                        suggestionDiv.appendChild(termSpan);
                        suggestionDiv.appendChild(categorySpan);
                        suggestionsContainer.appendChild(suggestionDiv);
                        
                    } catch (error) {
                        this.logError('Failed to create suggestion item', error);
                    }
                });
            }

            suggestionsContainer.style.display = 'block';
            
        } catch (error) {
            this.logError('Failed to show suggestions', error);
            suggestionsContainer.innerHTML = '<div class="no-suggestions"><span>Search temporarily unavailable</span></div>';
        }
    }

    highlightMatch(text, query) {
        try {
            // Sanitize inputs
            const sanitizedText = this.sanitizeHTML(text);
            const sanitizedQuery = this.sanitizeHTML(query);
            
            // Escape special regex characters
            const escapedQuery = sanitizedQuery.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
            
            if (!escapedQuery) return sanitizedText;
            
            const regex = new RegExp(`(${escapedQuery})`, 'gi');
            return sanitizedText.replace(regex, '<strong>$1</strong>');
        } catch (error) {
            this.logError('Failed to highlight match', error);
            return this.sanitizeHTML(text);
        }
    }

    hideSuggestions() {
        const suggestionsContainer = document.getElementById('searchSuggestions');
        if (suggestionsContainer) {
            suggestionsContainer.style.display = 'none';
        }
    }

    selectSuggestion(term) {
        const searchInput = document.getElementById('globalSearch');
        if (searchInput) {
            searchInput.value = term;
            this.hideSuggestions();
            this.performSearch(term);
        }
    }

    performSearch(query) {
        console.log(`🔍 Performing search for: ${query}`);
        // Implement actual search functionality here
        // This would integrate with your backend search system
    }

    showQuickAccess() {
        const quickAccess = document.querySelector('.search-quick-access');
        if (quickAccess) {
            quickAccess.style.display = 'flex';
        }
    }

    hideQuickAccess() {
        const quickAccess = document.querySelector('.search-quick-access');
        if (quickAccess) {
            quickAccess.style.display = 'none';
        }
    }

    quickSearch(category) {
        const searchInput = document.getElementById('globalSearch');
        if (searchInput) {
            searchInput.value = category;
            this.performSearch(category);
            this.hideQuickAccess();
        }
    }

    focusSearch() {
        const searchInput = document.getElementById('globalSearch');
        if (searchInput) {
            searchInput.focus();
            searchInput.select();
        }
    }

    // Voice Search
    startVoiceSearch() {
        if (!('webkitSpeechRecognition' in window) && !('SpeechRecognition' in window)) {
            alert('Voice search is not supported in your browser');
            return;
        }

        const recognition = new (window.SpeechRecognition || window.webkitSpeechRecognition)();
        recognition.lang = 'en-US';
        recognition.continuous = false;
        recognition.interimResults = false;

        recognition.onstart = () => {
            console.log('🎤 Voice search started...');
            const voiceBtn = document.querySelector('.search-voice-btn');
            if (voiceBtn) {
                voiceBtn.style.background = '#f44336';
                voiceBtn.style.color = 'white';
            }
        };

        recognition.onresult = (event) => {
            const query = event.results[0][0].transcript;
            console.log(`🎤 Voice search result: ${query}`);
            
            const searchInput = document.getElementById('globalSearch');
            if (searchInput) {
                searchInput.value = query;
                this.performSearch(query);
            }
        };

        recognition.onend = () => {
            console.log('🎤 Voice search ended');
            const voiceBtn = document.querySelector('.search-voice-btn');
            if (voiceBtn) {
                voiceBtn.style.background = 'transparent';
                voiceBtn.style.color = 'rgba(255, 255, 255, 0.6)';
            }
        };

        recognition.onerror = (event) => {
            console.error('Voice search error:', event.error);
        };

        recognition.start();
    }

    // Filter Management
    openSearchFilters() {
        console.log('🔧 Opening search filters...');
        // Implement filter modal/dropdown here
    }

    // Update Management
    showUpdateWarning() {
        if (!this.updateAvailable) return;

        const warningBanner = document.getElementById('updateWarning');
        if (warningBanner) {
            warningBanner.style.display = 'block';
            
            // Adjust main content margin
            const dashboardContent = document.querySelector('.dashboard-content');
            if (dashboardContent) {
                dashboardContent.style.marginTop = '170px'; // Account for banner height
            }
        }
    }

    startUpdate() {
        console.log('🚀 Starting system update...');
        this.showUpdateModal();
    }

    scheduleUpdate() {
        console.log('⏰ Scheduling system update...');
        this.showScheduleModal();
    }

    dismissUpdate() {
        const warningBanner = document.getElementById('updateWarning');
        if (warningBanner) {
            warningBanner.style.display = 'none';
            
            // Reset main content margin
            const dashboardContent = document.querySelector('.dashboard-content');
            if (dashboardContent) {
                dashboardContent.style.marginTop = '100px';
            }
        }
        this.updateAvailable = false;
    }

    showUpdateModal() {
        // Create and show update manager modal
        const modal = document.createElement('div');
        modal.className = 'update-modal';
        modal.innerHTML = `
            <div class="update-modal-content">
                <div class="update-modal-header">
                    <h2>System Update Manager</h2>
                    <button class="close-modal" onclick="this.closest('.update-modal').remove()">
                        <i class="fas fa-times"></i>
                    </button>
                </div>
                <div class="update-modal-body">
                    <div class="update-progress">
                        <div class="progress-bar">
                            <div class="progress-fill" id="updateProgress"></div>
                        </div>
                        <div class="progress-text">Preparing update...</div>
                    </div>
                </div>
            </div>
        `;
        
        document.body.appendChild(modal);
        this.simulateUpdate();
    }

    simulateUpdate() {
        const progressFill = document.getElementById('updateProgress');
        const progressText = document.querySelector('.progress-text');
        
        let progress = 0;
        const steps = [
            'Downloading updates...',
            'Verifying integrity...',
            'Backing up current version...',
            'Installing updates...',
            'Updating configurations...',
            'Restarting services...',
            'Update complete!'
        ];
        
        const updateInterval = setInterval(() => {
            progress += Math.random() * 15;
            if (progress > 100) progress = 100;
            
            if (progressFill) {
                progressFill.style.width = `${progress}%`;
            }
            
            if (progressText) {
                const stepIndex = Math.floor((progress / 100) * (steps.length - 1));
                progressText.textContent = steps[stepIndex];
            }
            
            if (progress >= 100) {
                clearInterval(updateInterval);
                setTimeout(() => {
                    const modal = document.querySelector('.update-modal');
                    if (modal) modal.remove();
                    this.dismissUpdate();
                    this.showSuccessNotification('System updated successfully!');
                }, 2000);
            }
        }, 500);
    }

    showScheduleModal() {
        // Implementation for scheduling updates
        console.log('📅 Showing schedule modal...');
    }

    // Status Updates
    startStatusUpdates() {
        setInterval(() => {
            this.updateSystemStatus();
            this.updateNotificationCount();
        }, 30000); // Update every 30 seconds
    }

    updateSystemStatus() {
        const statusLight = document.querySelector('.status-light');
        const statusText = document.querySelector('.status-text');
        
        // Simulate status checks
        const statuses = ['FORTRESS', 'SECURE', 'OPTIMAL', 'CHAMPION'];
        const currentStatus = statuses[Math.floor(Math.random() * statuses.length)];
        
        if (statusText) {
            statusText.textContent = currentStatus;
        }
    }

    updateNotificationCount() {
        // Simulate new notifications
        if (Math.random() < 0.1) { // 10% chance of new notification
            const countElement = document.getElementById('notificationCount');
            if (countElement) {
                const currentCount = parseInt(countElement.textContent) || 0;
                countElement.textContent = currentCount + 1;
                countElement.style.display = 'flex';
            }
        }
    }

    // Utility Methods
    closeAllMenus() {
        this.closeUserMenu();
        this.closeNotificationMenu();
        this.hideSuggestions();
        this.hideQuickAccess();
    }

    showSuccessNotification(message) {
        const notification = document.createElement('div');
        notification.className = 'success-notification';
        notification.innerHTML = `
            <i class="fas fa-check-circle"></i>
            <span>${message}</span>
        `;
        
        notification.style.cssText = `
            position: fixed;
            top: 120px;
            right: 2rem;
            background: linear-gradient(45deg, #4caf50, #388e3c);
            color: white;
            padding: 1rem 2rem;
            border-radius: 12px;
            box-shadow: 0 8px 25px rgba(76, 175, 80, 0.3);
            z-index: 10000;
            display: flex;
            align-items: center;
            gap: 0.75rem;
            font-weight: 600;
            animation: slideInRight 0.3s ease-out;
        `;
        
        document.body.appendChild(notification);
        
        setTimeout(() => {
            notification.style.animation = 'slideOutRight 0.3s ease-in forwards';
            setTimeout(() => notification.remove(), 300);
        }, 4000);
    }

    // Logout functionality
    logout() {
        if (confirm('Are you sure you want to logout?')) {
            console.log('🚪 User logging out...');
            
            // Show logout animation
            const overlay = document.createElement('div');
            overlay.style.cssText = `
                position: fixed;
                top: 0;
                left: 0;
                width: 100%;
                height: 100%;
                background: linear-gradient(45deg, #8b3f8b, #ff69b4);
                z-index: 10000;
                display: flex;
                align-items: center;
                justify-content: center;
                color: white;
                font-size: 1.5rem;
                font-weight: 600;
                animation: fadeIn 0.5s ease-out;
            `;
            overlay.innerHTML = `
                <div style="text-align: center;">
                    <i class="fas fa-sign-out-alt fa-3x" style="margin-bottom: 1rem; animation: pulse 1s infinite;"></i>
                    <div>Logging out...</div>
                </div>
            `;
            
            document.body.appendChild(overlay);
            
            // Simulate logout process
            setTimeout(() => {
                // Redirect to login page or perform logout actions
                window.location.href = '/login.html';
            }, 2000);
        }
    }
}

// Global Functions (called from HTML)
function toggleUserMenu() {
    if (window.headerManager) {
        window.headerManager.toggleUserMenu();
    }
}

function toggleNotifications() {
    if (window.headerManager) {
        window.headerManager.toggleNotifications();
    }
}

function markAllRead() {
    if (window.headerManager) {
        window.headerManager.markAllRead();
    }
}

function quickSearch(category) {
    if (window.headerManager) {
        window.headerManager.quickSearch(category);
    }
}

function openSearchFilters() {
    if (window.headerManager) {
        window.headerManager.openSearchFilters();
    }
}

function startVoiceSearch() {
    if (window.headerManager) {
        window.headerManager.startVoiceSearch();
    }
}

function startUpdate() {
    if (window.headerManager) {
        window.headerManager.startUpdate();
    }
}

function scheduleUpdate() {
    if (window.headerManager) {
        window.headerManager.scheduleUpdate();
    }
}

function dismissUpdate() {
    if (window.headerManager) {
        window.headerManager.dismissUpdate();
    }
}

function logout() {
    if (window.headerManager) {
        window.headerManager.logout();
    }
}

function toggleQuickActions() {
    console.log('⚡ Opening quick actions...');
    // Implementation for quick actions menu
}

function openSettings() {
    console.log('⚙️ Opening system settings...');
    // Implementation for settings panel
}

// Additional CSS for animations
const additionalStyles = document.createElement('style');
additionalStyles.textContent = `
    @keyframes slideInRight {
        from { transform: translateX(100%); opacity: 0; }
        to { transform: translateX(0); opacity: 1; }
    }
    
    @keyframes slideOutRight {
        from { transform: translateX(0); opacity: 1; }
        to { transform: translateX(100%); opacity: 0; }
    }
    
    @keyframes fadeIn {
        from { opacity: 0; }
        to { opacity: 1; }
    }
    
    @keyframes pulse {
        0%, 100% { transform: scale(1); }
        50% { transform: scale(1.1); }
    }
    
    .update-modal {
        position: fixed;
        top: 0;
        left: 0;
        width: 100%;
        height: 100%;
        background: rgba(0, 0, 0, 0.8);
        z-index: 10000;
        display: flex;
        align-items: center;
        justify-content: center;
    }
    
    .update-modal-content {
        background: rgba(255, 255, 255, 0.12);
        backdrop-filter: blur(25px);
        border: 1px solid rgba(255, 255, 255, 0.15);
        border-radius: 20px;
        padding: 2rem;
        width: 90%;
        max-width: 500px;
        color: white;
    }
    
    .update-modal-header {
        display: flex;
        align-items: center;
        justify-content: space-between;
        margin-bottom: 2rem;
    }
    
    .close-modal {
        background: transparent;
        border: none;
        color: white;
        font-size: 1.5rem;
        cursor: pointer;
        padding: 0.5rem;
        border-radius: 50%;
        transition: background 0.2s ease;
    }
    
    .close-modal:hover {
        background: rgba(255, 255, 255, 0.1);
    }
    
    .progress-bar {
        width: 100%;
        height: 8px;
        background: rgba(255, 255, 255, 0.2);
        border-radius: 4px;
        overflow: hidden;
        margin-bottom: 1rem;
    }
    
    .progress-fill {
        height: 100%;
        background: linear-gradient(45deg, #8b3f8b, #ff69b4);
        width: 0%;
        transition: width 0.5s ease;
    }
    
    .progress-text {
        text-align: center;
        font-weight: 600;
        color: rgba(255, 255, 255, 0.9);
    }
    
    .suggestion-item {
        display: flex;
        align-items: center;
        gap: 1rem;
        padding: 0.75rem 1rem;
        cursor: pointer;
        transition: background 0.2s ease;
    }
    
    .suggestion-item:hover {
        background: rgba(139, 63, 139, 0.2);
    }
    
    .suggestion-term {
        flex: 1;
        color: white;
    }
    
    .suggestion-category {
        font-size: 0.8rem;
        color: rgba(255, 255, 255, 0.6);
        background: rgba(139, 63, 139, 0.2);
        padding: 0.25rem 0.5rem;
        border-radius: 8px;
    }
    
    .no-suggestions {
        display: flex;
        flex-direction: column;
        align-items: center;
        gap: 0.5rem;
        padding: 2rem;
        color: rgba(255, 255, 255, 0.6);
    }
`;
document.head.appendChild(additionalStyles);

// Initialize Header Manager when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    window.headerManager = new MedusaHeaderManager();
});

console.log('🔮 MedusaServ Header Scripts Loaded!');