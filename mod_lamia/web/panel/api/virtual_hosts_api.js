/**
 * Virtual Host Manager API v0.3.0c
 * Lightning-fast CRUD operations
 * © 2025 The Medusa Project
 */

const express = require('express');
const router = express.Router();

// In-memory storage for lightning-fast performance
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

// GET all virtual hosts
router.get('/api/virtualhosts', (req, res) => {
    res.json({
        status: 'success',
        data: virtualHosts,
        count: virtualHosts.length
    });
});

// GET single virtual host by ID
router.get('/api/virtualhosts/:id', (req, res) => {
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

// POST create new virtual host
router.post('/api/virtualhosts', (req, res) => {
    const { title, folder, description, status, ssl_enabled, port } = req.body;
    
    // Validation
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

// PUT update virtual host
router.put('/api/virtualhosts/:id', (req, res) => {
    const id = parseInt(req.params.id);
    const hostIndex = virtualHosts.findIndex(h => h.id === id);
    
    if (hostIndex === -1) {
        return res.status(404).json({
            status: 'error',
            message: 'Virtual host not found'
        });
    }
    
    const { title, folder, description, status, ssl_enabled, port } = req.body;
    
    // Update only provided fields
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

// DELETE virtual host
router.delete('/api/virtualhosts/:id', (req, res) => {
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

module.exports = router;