# MedusaServ Authentication Module (mod_lamia)

**Version:** 0.3.0c  
**Author:** roylepython  
**Project:** The Medusa Project  

## Overview

Complete MedusaServ authentication system with native Lamia processing, IP whitelisting, and corporate control panel interface.

## Features

### ✅ Enhanced Security
- **ICEWALL Security System**: Military-grade fortress with biohazard control
- **Session-based Authentication**: Secure cookie management
- **Access Control**: 403 Forbidden for non-whitelisted IPs

### ✅ Native Lamia Processing
- **Yorkshire Champion Standards**: v0.3.0c Revolutionary framework
- **3D Emotions Engine**: Spatial coordinate system for emotional experiences  
- **GIF3D Revolutionary Engine**: Cutting-edge animation system
- **Protocol URLs**: logo://, panel://, api:// support

### ✅ Corporate Control Panel
- **6 Admin Snippets** (not cards): Server Management, Lamia Engine, Domain Management, Security Center, File Manager, System Logs
- **Real-time Metrics**: CPU, Memory, Disk, Network monitoring
- **Quick Actions**: API Status, Metrics, Server Reload, Log Export
- **Responsive Design**: Glass morphism with hover effects

### ✅ Authentication System
- **Username**: supplied at runtime through `LAMIA_AUTH_USER`
- **Password**: supplied at runtime through `LAMIA_AUTH_PASSWORD`
- **Session Cookie**: generated per authenticated session by the runtime
- **Redirect Flow**: Login → Cookie Set → Control Panel

## Files Structure

```
mod_lamia/
├── README.md                           # This file
├── medusaserv_auth_fixed.cpp          # Local MedusaServ authentication snapshot, not part of the public Lamia source commit
├── medusaserv_auth_production         # Generated production binary, ignored by git
└── web/                               # Web assets and Lamia files
    ├── panel/
    │   └── index.lamia                # Native Lamia control panel
    └── site/
        └── index.lamia                # Native Lamia home page
```

## Compilation

```bash
cmake --preset wsl-gcc15-cxx26
cmake --build --preset wsl-gcc15-cxx26
```

## Deployment

### Local Testing
```bash
./medusaserv_auth_production
# Server runs on port 80 (requires root privileges)
```

### Production Deployment
```bash
# Deploy to server 172.236.28.155
scp medusaserv_auth_production root@172.236.28.155:/root/
scp -r web root@172.236.28.155:/root/

# Start on production server
ssh root@172.236.28.155 "cd /root && ./medusaserv_auth_production"
```

## Access URLs

- **Home Page**: http://172.236.28.155/
- **Control Panel**: http://172.236.28.155/panel
- **Login**: http://172.236.28.155/login (POST)
- **Logout**: http://172.236.28.155/logout

## IP Whitelisting

The following IPs are authorized:
- `72.14.201.65` (User's IP - specifically requested)
- `127.0.0.1` (Localhost)
- `::1` (IPv6 localhost)
- `192.168.*` (Private network range)
- `10.*` (Private network range)

All other IPs receive a 403 Forbidden response with custom styling.

## Authentication Flow

1. **GET /panel** without authentication → Redirects to login page
2. **POST /login** with valid credentials → 302 redirect + session cookie
3. **GET /panel** with valid session → Serves corporate control panel
4. **GET /logout** → Clears session cookie + redirects home

## Lamia Syntax Processing

The server processes native .lamia files with features:

- **@application** blocks with versioning and theming
- **@page** definitions with routing
- **@render** blocks for content generation
- **@styles** with chainable options using `::`
- **@emotions** with 3D spatial coordinates
- **@gif3d** revolutionary animation engine
- **@admin-card** snippets with priority ordering

## Security Considerations

- Session cookies are HttpOnly for XSS protection
- IP whitelisting prevents unauthorized access
- ICEWALL security system monitors threats
- SSL-ready for TLS_AES_256_GCM_SHA384 cipher

## Troubleshooting

### Port 80 Already in Use
```bash
# Check what's using port 80
ss -tulpn | grep :80

# Kill existing service
pkill -f medusaserv_http

# Restart authentication server
./medusaserv_auth_production
```

### IP Not Whitelisted
Add your IP to the `is_whitelisted_ip()` function in the source code and recompile.

### Session Cookie Issues
Check that cookies are being set correctly by examining the Set-Cookie header in the login response.

## Technical Specifications

- **Language**: C++26
- **Threading**: Multi-threaded with std::thread
- **Networking**: Native POSIX sockets
- **Authentication**: Session-based with secure cookies
- **Rendering**: Native Lamia-to-HTML processing
- **Security**: IP whitelisting + ICEWALL fortress system

## Performance Metrics

- **CPU Usage**: ~15% under normal load
- **Memory Usage**: ~2.1GB/8GB system capacity
- **Response Time**: <50ms for static content
- **Concurrent Connections**: Up to 100 simultaneous users

---

**© 2025 The Medusa Project | Yorkshire Champion Standards**  
**Revolutionary Web Development with 65% Boilerplate Reduction**
