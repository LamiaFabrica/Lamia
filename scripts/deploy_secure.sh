#!/bin/bash
# Secure Deployment Script
# Uses environment variables for credentials

if [ -z "$MEDUSA_SSH_HOST" ] || [ -z "$MEDUSA_SSH_USER" ] || [ -z "$MEDUSA_SSH_PASS" ]; then
    echo "❌ Security Error: Credentials not found in environment"
    echo "Please set MEDUSA_SSH_HOST, MEDUSA_SSH_USER, MEDUSA_SSH_PASS"
    exit 1
fi

echo "🔐 Deploying with secure credentials..."
sshpass -p "$MEDUSA_SSH_PASS" scp $1 $MEDUSA_SSH_USER@$MEDUSA_SSH_HOST:/root/
sshpass -p "$MEDUSA_SSH_PASS" scp -r web $MEDUSA_SSH_USER@$MEDUSA_SSH_HOST:/root/
echo "✅ Secure deployment complete"