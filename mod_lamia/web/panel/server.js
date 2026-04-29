/**
 * MedusaServ Panel Server v0.3.0c
 * With Virtual Host Manager Integration
 * © 2025 The Medusa Project
 */

const express = require('express');
const path = require('path');
const fs = require('fs');
const app = express();

// Middleware
app.use(express.json());
app.use(express.static(path.join(__dirname)));

// In-memory storage for virtual hosts
let virtualHosts = [
    {
        id: 1,
        title: "Default Website",
        folder: "/web/site",
        description: "The doorway to your perfect first website",
        status: "active",
        ssl_enabled: false,
        port: 80,
        created: new Date().toISOString(),
        modified: new Date().toISOString()
    }
];

let nextId = 2;

// Virtual Host API Routes
app.get('/api/virtualhosts', (req, res) => {
    res.json({
        status: 'success',
        data: virtualHosts,
        count: virtualHosts.length
    });
});

app.get('/api/virtualhosts/:id', (req, res) => {
    const id = parseInt(req.params.id);
    const host = virtualHosts.find(h => h.id === id);
    
    if (host) {
        res.json({
            status: 'success',
            data: host
        });
    } else {
        res.status(404).json({
            status: 'error',
            message: 'Virtual host not found'
        });
    }
});

app.post('/api/virtualhosts', (req, res) => {
    const { title, folder, description, status, ssl_enabled, port } = req.body;
    
    if (!title || !folder) {
        return res.status(400).json({
            status: 'error',
            message: 'Title and Folder are required fields'
        });
    }
    
    const newHost = {
        id: nextId++,
        title,
        folder,
        description: description || '',
        status: status || 'active',
        ssl_enabled: ssl_enabled || false,
        port: port || 80,
        created: new Date().toISOString(),
        modified: new Date().toISOString()
    };
    
    virtualHosts.push(newHost);
    
    res.status(201).json({
        status: 'success',
        message: 'Virtual host created successfully',
        data: newHost
    });
});

app.put('/api/virtualhosts/:id', (req, res) => {
    const id = parseInt(req.params.id);
    const hostIndex = virtualHosts.findIndex(h => h.id === id);
    
    if (hostIndex === -1) {
        return res.status(404).json({
            status: 'error',
            message: 'Virtual host not found'
        });
    }
    
    const { title, folder, description, status, ssl_enabled, port } = req.body;
    
    if (title !== undefined) virtualHosts[hostIndex].title = title;
    if (folder !== undefined) virtualHosts[hostIndex].folder = folder;
    if (description !== undefined) virtualHosts[hostIndex].description = description;
    if (status !== undefined) virtualHosts[hostIndex].status = status;
    if (ssl_enabled !== undefined) virtualHosts[hostIndex].ssl_enabled = ssl_enabled;
    if (port !== undefined) virtualHosts[hostIndex].port = port;
    
    virtualHosts[hostIndex].modified = new Date().toISOString();
    
    res.json({
        status: 'success',
        message: 'Virtual host updated successfully',
        data: virtualHosts[hostIndex]
    });
});

app.delete('/api/virtualhosts/:id', (req, res) => {
    const id = parseInt(req.params.id);
    const hostIndex = virtualHosts.findIndex(h => h.id === id);
    
    if (hostIndex === -1) {
        return res.status(404).json({
            status: 'error',
            message: 'Virtual host not found'
        });
    }
    
    const deletedHost = virtualHosts.splice(hostIndex, 1)[0];
    
    res.json({
        status: 'success',
        message: 'Virtual host deleted successfully',
        data: deletedHost
    });
});

// Serve the Virtual Host Manager page
app.get('/virtualhosts', (req, res) => {
    res.sendFile(path.join(__dirname, 'virtual_host_manager.html'));
});

// Main panel route
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});

// Start server
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log(`🏆 MedusaServ Panel Server v0.3.0c`);
    console.log(`✅ Virtual Host Manager API ready`);
    console.log(`🚀 Server running on http://localhost:${PORT}`);
    console.log(`🌐 Virtual Host Manager: http://localhost:${PORT}/virtualhosts`);
    console.log(`📊 Main Panel: http://localhost:${PORT}/`);
    console.log(`\n⚡ Lightning-fast CRUD operations enabled`);
    console.log(`📁 Default website folder: /web/site`);
});