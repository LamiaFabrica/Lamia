#!/bin/bash
# Secure Server Startup Script

if [ -z "$MEDUSA_SSH_HOST" ] || [ -z "$MEDUSA_SSH_USER" ] || [ -z "$MEDUSA_SSH_PASS" ]; then
    echo "❌ Security Error: Credentials not found in environment"
    exit 1
fi

echo "🔐 Starting server with secure credentials..."
sshpass -p "$MEDUSA_SSH_PASS" ssh $MEDUSA_SSH_USER@$MEDUSA_SSH_HOST "cd /root && pkill -f medusaserv || true && nohup ./$1 > medusaserv_auth.log 2>&1 &"
echo "✅ Secure server startup complete"