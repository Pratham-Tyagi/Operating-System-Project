<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>File Management System</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            color: #333;
        }

        .container {
            background: rgba(255, 255, 255, 0.95);
            backdrop-filter: blur(10px);
            border-radius: 20px;
            box-shadow: 0 20px 40px rgba(0, 0, 0, 0.1);
            padding: 2rem;
            width: 90%;
            max-width: 800px;
            min-height: 500px;
            transition: all 0.3s ease;
        }

        .container:hover {
            box-shadow: 0 25px 50px rgba(0, 0, 0, 0.15);
        }

        .header {
            text-align: center;
            margin-bottom: 2rem;
        }

        .header h1 {
            color: #5a67d8;
            font-size: 2.5rem;
            margin-bottom: 0.5rem;
            text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.1);
        }

        .header p {
            color: #666;
            font-size: 1.1rem;
        }

        .auth-section, .main-section {
            display: none;
        }

        .auth-section.active, .main-section.active {
            display: block;
        }

        .form-group {
            margin-bottom: 1.5rem;
        }

        label {
            display: block;
            margin-bottom: 0.5rem;
            font-weight: 600;
            color: #4a5568;
        }

        input[type="text"], input[type="password"], textarea {
            width: 100%;
            padding: 0.75rem;
            border: 2px solid #e2e8f0;
            border-radius: 10px;
            font-size: 1rem;
            transition: all 0.3s ease;
            background: rgba(255, 255, 255, 0.8);
        }

        input[type="text"]:focus, input[type="password"]:focus, textarea:focus {
            outline: none;
            border-color: #5a67d8;
            box-shadow: 0 0 0 3px rgba(90, 103, 216, 0.1);
            transform: translateY(-2px);
        }

        .btn {
            background: linear-gradient(135deg, #5a67d8, #667eea);
            color: white;
            border: none;
            padding: 0.75rem 1.5rem;
            border-radius: 10px;
            font-size: 1rem;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s ease;
            margin: 0.25rem;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }

        .btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 10px 20px rgba(90, 103, 216, 0.3);
        }

        .btn-secondary {
            background: linear-gradient(135deg, #718096, #4a5568);
        }

        .btn-danger {
            background: linear-gradient(135deg, #e53e3e, #c53030);
        }

        .btn-success {
            background: linear-gradient(135deg, #38a169, #2f855a);
        }

        .menu-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            grid-gap: 1rem;
            margin: 2rem 0;
        }

        .menu-item {
            background: rgba(255, 255, 255, 0.8);
            border: 2px solid #e2e8f0;
            border-radius: 15px;
            padding: 1.5rem;
            text-align: center;
            cursor: pointer;
            transition: all 0.3s ease;
        }

        .menu-item:hover {
            border-color: #5a67d8;
            transform: translateY(-5px);
            box-shadow: 0 10px 25px rgba(90, 103, 216, 0.2);
        }

        .menu-item h3 {
            color: #5a67d8;
            margin-bottom: 0.5rem;
        }

        .status-bar {
            background: rgba(90, 103, 216, 0.1);
            border-radius: 10px;
            padding: 1rem;
            margin-bottom: 2rem;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .user-info {
            font-weight: 600;
            color: #5a67d8;
        }

        .admin-badge {
            background: linear-gradient(135deg, #ed8936, #dd6b20);
            color: white;
            padding: 0.25rem 0.75rem;
            border-radius: 20px;
            font-size: 0.8rem;
            font-weight: 600;
        }

        .file-list {
            background: rgba(255, 255, 255, 0.8);
            border-radius: 10px;
            padding: 1rem;
            margin: 1rem 0;
            max-height: 300px;
            overflow-y: auto;
        }

        .file-item {
            padding: 0.5rem;
            border-bottom: 1px solid #e2e8f0;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .file-item:last-child {
            border-bottom: none;
        }

        .modal {
            display: none;
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background: rgba(0, 0, 0, 0.5);
            backdrop-filter: blur(5px);
            z-index: 1000;
        }

        .modal.active {
            display: flex;
            align-items: center;
            justify-content: center;
        }

        .modal-content {
            background: white;
            border-radius: 20px;
            padding: 2rem;
            width: 90%;
            max-width: 500px;
            max-height: 80vh;
            overflow-y: auto;
        }

        .alert {
            padding: 1rem;
            border-radius: 10px;
            margin: 1rem 0;
            font-weight: 600;
        }

        .alert-success {
            background: rgba(56, 161, 105, 0.1);
            color: #2f855a;
            border: 1px solid rgba(56, 161, 105, 0.3);
        }

        .alert-error {
            background: rgba(229, 62, 62, 0.1);
            color: #c53030;
            border: 1px solid rgba(229, 62, 62, 0.3);
        }

        .loading {
            display: inline-block;
            width: 20px;
            height: 20px;
            border: 3px solid #f3f3f3;
            border-top: 3px solid #5a67d8;
            border-radius: 50%;
            animation: spin 1s linear infinite;
        }

        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }

        .fade-in {
            animation: fadeIn 0.5s ease-in;
        }

        @keyframes fadeIn {
            from { opacity: 0; transform: translateY(20px); }
            to { opacity: 1; transform: translateY(0); }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>📁 File Manager</h1>
            <p>Secure File Management System</p>
        </div>

        <!-- Authentication Section -->
        <div class="auth-section active" id="authSection">
            <div class="form-group">
                <button class="btn" onclick="showLogin()">Login</button>
                <button class="btn btn-secondary" onclick="showRegister()">Register</button>
            </div>

            <div id="loginForm" style="display: none;">
                <h2>Login</h2>
                <div class="form-group">
                    <label>Username:</label>
                    <input type="text" id="loginUsername" placeholder="Enter username">
                </div>
                <div class="form-group">
                    <label>Password:</label>
                    <input type="password" id="loginPassword" placeholder="Enter password">
                </div>
                <div id="adminPasswordGroup" style="display: none;">
                    <div class="form-group">
                        <label>Admin Password:</label>
                        <input type="password" id="adminPassword" placeholder="Enter admin password">
                    </div>
                </div>
                <button class="btn" onclick="login()">Login</button>
            </div>

            <div id="registerForm" style="display: none;">
                <h2>Register New User</h2>
                <div class="form-group">
                    <label>Username:</label>
                    <input type="text" id="regUsername" placeholder="Enter username">
                </div>
                <div class="form-group">
                    <label>Password:</label>
                    <input type="password" id="regPassword" placeholder="Enter password">
                </div>
                <button class="btn btn-success" onclick="register()">Register</button>
            </div>
        </div>

        <!-- Main System Section -->
        <div class="main-section" id="mainSection">
            <div class="status-bar">
                <div class="user-info">
                    Welcome, <span id="currentUser"></span>
                    <span id="adminBadge" class="admin-badge" style="display: none;">ADMIN</span>
                </div>
                <button class="btn btn-secondary" onclick="logout()">Logout</button>
            </div>

            <div class="menu-grid">
                <div class="menu-item" onclick="showModal('createFileModal')">
                    <h3>📄 Create File</h3>
                    <p>Create a new file</p>
                </div>
                <div class="menu-item" onclick="showModal('writeFileModal')">
                    <h3>✍️ Write to File</h3>
                    <p>Add content to file</p>
                </div>
                <div class="menu-item" onclick="showModal('modifyFileModal')">
                    <h3>📝 Modify File</h3>
                    <p>Edit existing file</p>
                </div>
                <div class="menu-item" onclick="showModal('searchModal')">
                    <h3>🔍 Search Content</h3>
                    <p>Find text in files</p>
                </div>
                <div class="menu-item" onclick="showModal('readFileModal')">
                    <h3>📖 Read File</h3>
                    <p>View file contents</p>
                </div>
                <div class="menu-item" onclick="showModal('deleteFileModal')">
                    <h3>🗑️ Delete File</h3>
                    <p>Remove file</p>
                </div>
            </div>

            <div class="file-list">
                <h3>Your Files:</h3>
                <div id="fileListContent">
                    <p>No files created yet.</p>
                </div>
            </div>
        </div>

        <div id="alerts"></div>
    </div>

    <!-- Modals for File Operations -->
    <div class="modal" id="createFileModal">
        <div class="modal-content">
            <h2>Create New File</h2>
            <div class="form-group">
                <label>File Name:</label>
                <input type="text" id="createFileName" placeholder="Enter file name">
            </div>
            <div id="ownerGroup" style="display: none;">
                <div class="form-group">
                    <label>Owner Username:</label>
                    <input type="text" id="fileOwner" placeholder="Enter owner username">
                </div>
            </div>
            <button class="btn" onclick="createFile()">Create File</button>
            <button class="btn btn-secondary" onclick="closeModal('createFileModal')">Cancel</button>
        </div>
    </div>

    <div class="modal" id="writeFileModal">
        <div class="modal-content">
            <h2>Write to File</h2>
            <div class="form-group">
                <label>File Name:</label>
                <input type="text" id="writeFileName" placeholder="Enter file name">
            </div>
            <div class="form-group">
                <label>Content:</label>
                <textarea id="writeContent" rows="5" placeholder="Enter content to write"></textarea>
            </div>
            <button class="btn" onclick="writeToFile()">Write Content</button>
            <button class="btn btn-secondary" onclick="closeModal('writeFileModal')">Cancel</button>
        </div>
    </div>

    <div class="modal" id="modifyFileModal">
        <div class="modal-content">
            <h2>Modify File</h2>
            <div class="form-group">
                <label>File Name:</label>
                <input type="text" id="modifyFileName" placeholder="Enter file name">
            </div>
            <div class="form-group">
                <label>New Content:</label>
                <textarea id="modifyContent" rows="5" placeholder="Enter new content"></textarea>
            </div>
            <button class="btn" onclick="modifyFile()">Modify File</button>
            <button class="btn btn-secondary" onclick="closeModal('modifyFileModal')">Cancel</button>
        </div>
    </div>

    <div class="modal" id="searchModal">
        <div class="modal-content">
            <h2>Search Content</h2>
            <div class="form-group">
                <label>File Name:</label>
                <input type="text" id="searchFileName" placeholder="Enter file name">
            </div>
            <div class="form-group">
                <label>Search Keyword:</label>
                <input type="text" id="searchKeyword" placeholder="Enter keyword to search">
            </div>
            <div id="searchResults"></div>
            <button class="btn" onclick="searchContent()">Search</button>
            <button class="btn btn-secondary" onclick="closeModal('searchModal')">Cancel</button>
        </div>
    </div>

    <div class="modal" id="readFileModal">
        <div class="modal-content">
            <h2>Read File</h2>
            <div class="form-group">
                <label>File Name:</label>
                <input type="text" id="readFileName" placeholder="Enter file name">
            </div>
            <div id="fileContent" style="background: #f7fafc; padding: 1rem; border-radius: 10px; margin: 1rem 0; min-height: 100px; white-space: pre-wrap;"></div>
            <button class="btn" onclick="readFile()">Read File</button>
            <button class="btn btn-secondary" onclick="closeModal('readFileModal')">Cancel</button>
        </div>
    </div>

    <div class="modal" id="deleteFileModal">
        <div class="modal-content">
            <h2>Delete File</h2>
            <div class="form-group">
                <label>File Name:</label>
                <input type="text" id="deleteFileName" placeholder="Enter file name">
            </div>
            <p style="color: #e53e3e; margin: 1rem 0;">⚠️ This action cannot be undone!</p>
            <button class="btn btn-danger" onclick="deleteFile()">Delete File</button>
            <button class="btn btn-secondary" onclick="closeModal('deleteFileModal')">Cancel</button>
        </div>
    </div>

    <script>
        // Application State
        let currentUser = '';
        let isAdmin = false;
        let users = {}; // Store encrypted users
        let files = {}; // Store files in memory
        const key = 'K'; // XOR key for encryption
        const adminPassword = 'admin123';

        // XOR Encryption/Decryption
        function encryptDecrypt(input) {
            let output = '';
            for (let i = 0; i < input.length; i++) {
                output += String.fromCharCode(input.charCodeAt(i) ^ key.charCodeAt(0));
            }
            return output;
        }

        // Show/Hide Authentication Forms
        function showLogin() {
            document.getElementById('loginForm').style.display = 'block';
            document.getElementById('registerForm').style.display = 'none';
        }

        function showRegister() {
            document.getElementById('loginForm').style.display = 'none';
            document.getElementById('registerForm').style.display = 'block';
        }

        // Register New User
        function register() {
            const username = document.getElementById('regUsername').value.trim();
            const password = document.getElementById('regPassword').value.trim();

            if (!username || !password) {
                showAlert('Please fill in all fields', 'error');
                return;
            }

            const encUsername = encryptDecrypt(username);
            const encPassword = encryptDecrypt(password);
            
            users[encUsername] = encPassword;
            
            showAlert('User registered successfully! You can now login.', 'success');
            document.getElementById('regUsername').value = '';
            document.getElementById('regPassword').value = '';
            showLogin();
        }

        // Login Function
        function login() {
            const username = document.getElementById('loginUsername').value.trim();
            const password = document.getElementById('loginPassword').value.trim();

            if (!username || !password) {
                showAlert('Please fill in all fields', 'error');
                return;
            }

            const encUsername = encryptDecrypt(username);
            const encPassword = encryptDecrypt(password);

            if (users[encUsername] && users[encUsername] === encPassword) {
                currentUser = username;
                
                if (username === 'admin') {
                    document.getElementById('adminPasswordGroup').style.display = 'block';
                    const adminPass = document.getElementById('adminPassword').value.trim();
                    
                    if (adminPass === adminPassword) {
                        isAdmin = true;
                        showAlert('Admin verification successful!', 'success');
                    } else if (adminPass) {
                        showAlert('Admin verification failed. Limited access granted.', 'error');
                        isAdmin = false;
                    } else {
                        showAlert('Please enter admin password', 'error');
                        return;
                    }
                }

                document.getElementById('currentUser').textContent = currentUser;
                if (isAdmin) {
                    document.getElementById('adminBadge').style.display = 'inline-block';
                    document.getElementById('ownerGroup').style.display = 'block';
                }

                document.getElementById('authSection').classList.remove('active');
                document.getElementById('mainSection').classList.add('active');
                document.getElementById('mainSection').classList.add('fade-in');
                
                updateFileList();
                showAlert(`Welcome, ${currentUser}!`, 'success');
            } else {
                showAlert('Invalid credentials!', 'error');
            }
        }

        // Logout Function
        function logout() {
            currentUser = '';
            isAdmin = false;
            document.getElementById('adminBadge').style.display = 'none';
            document.getElementById('ownerGroup').style.display = 'none';
            document.getElementById('authSection').classList.add('active');
            document.getElementById('mainSection').classList.remove('active');
            
            // Clear forms
            document.getElementById('loginUsername').value = '';
            document.getElementById('loginPassword').value = '';
            document.getElementById('adminPassword').value = '';
            document.getElementById('adminPasswordGroup').style.display = 'none';
        }

        // Admin Verification
        function verifyAdmin() {
            if (!isAdmin) return true;
            
            const response = prompt("Are you genuine? (yes/no):");
            if (response && response.toLowerCase() === 'yes') {
                return true;
            } else {
                showAlert('Admin verification failed. Access denied.', 'error');
                return false;
            }
        }

        // Get Full File Path
        function getFullPath(filename, owner = null) {
            const fileOwner = owner || (isAdmin ? document.getElementById('fileOwner').value.trim() || currentUser : currentUser);
            return `${fileOwner}_${filename}`;
        }

        // File Operations
        function createFile() {
            if (!verifyAdmin()) return;
            
            const filename = document.getElementById('createFileName').value.trim();
            if (!filename) {
                showAlert('Please enter a file name', 'error');
                return;
            }

            const fullPath = getFullPath(filename);
            files[fullPath] = '';
            
            showAlert(`File '${fullPath}' created successfully!`, 'success');
            document.getElementById('createFileName').value = '';
            document.getElementById('fileOwner').value = '';
            closeModal('createFileModal');
            updateFileList();
        }

        function writeToFile() {
            const filename = document.getElementById('writeFileName').value.trim();
            const content = document.getElementById('writeContent').value;
            
            if (!filename) {
                showAlert('Please enter a file name', 'error');
                return;
            }

            const fullPath = getFullPath(filename);
            if (!files.hasOwnProperty(fullPath)) {
                showAlert('File not found', 'error');
                return;
            }

            files[fullPath] += content + '\n';
            showAlert('Content written successfully!', 'success');
            document.getElementById('writeFileName').value = '';
            document.getElementById('writeContent').value = '';
            closeModal('writeFileModal');
        }

        function modifyFile() {
            const filename = document.getElementById('modifyFileName').value.trim();
            const content = document.getElementById('modifyContent').value;
            
            if (!filename) {
                showAlert('Please enter a file name', 'error');
                return;
            }

            const fullPath = getFullPath(filename);
            if (!files.hasOwnProperty(fullPath)) {
                showAlert('File not found', 'error');
                return;
            }

            files[fullPath] = content;
            showAlert('File modified successfully!', 'success');
            document.getElementById('modifyFileName').value = '';
            document.getElementById('modifyContent').value = '';
            closeModal('modifyFileModal');
        }

        function searchContent() {
            const filename = document.getElementById('searchFileName').value.trim();
            const keyword = document.getElementById('searchKeyword').value.trim();
            
            if (!filename || !keyword) {
                showAlert('Please enter both file name and keyword', 'error');
                return;
            }

            const fullPath = getFullPath(filename);
            if (!files.hasOwnProperty(fullPath)) {
                showAlert('File not found', 'error');
                return;
            }

            const content = files[fullPath];
            const lines = content.split('\n');
            const results = lines.filter(line => line.includes(keyword));
            
            const resultsDiv = document.getElementById('searchResults');
            if (results.length > 0) {
                resultsDiv.innerHTML = '<h4>Search Results:</h4>' + 
                    results.map(line => `<p style="background: #fff3cd; padding: 0.5rem; margin: 0.25rem 0; border-radius: 5px;">${line}</p>`).join('');
            } else {
                resultsDiv.innerHTML = '<p>Keyword not found in file.</p>';
            }
        }

        function readFile() {
            const filename = document.getElementById('readFileName').value.trim();
            
            if (!filename) {
                showAlert('Please enter a file name', 'error');
                return;
            }

            const fullPath = getFullPath(filename);
            if (!files.hasOwnProperty(fullPath)) {
                showAlert('File not found', 'error');
                return;
            }

            const content = files[fullPath] || '(Empty file)';
            document.getElementById('fileContent').textContent = content;
        }

        function deleteFile() {
            if (!verifyAdmin()) return;
            
            const filename = document.getElementById('deleteFileName').value.trim();
            
            if (!filename) {
                showAlert('Please enter a file name', 'error');
                return;
            }

            const fullPath = getFullPath(filename);
            if (!files.hasOwnProperty(fullPath)) {
                showAlert('File not found', 'error');
                return;
            }

            delete files[fullPath];
            showAlert(`File '${fullPath}' deleted successfully!`, 'success');
            document.getElementById('deleteFileName').value = '';
            closeModal('deleteFileModal');
            updateFileList();
        }

        // Update File List Display
        function updateFileList() {
            const fileListContent = document.getElementById('fileListContent');
            const userFiles = Object.keys(files).filter(path => 
                isAdmin || path.startsWith(currentUser + '_')
            );

            if (userFiles.length === 0) {
                fileListContent.innerHTML = '<p>No files found.</p>';
            } else {
                fileListContent.innerHTML = userFiles.map(path => {
                    const displayName = path.split('_').slice(1).join('_');
                    const owner = path.split('_')[0];
                    return `<div class="file-item">
                        <span>📄 ${displayName} <small>(by ${owner})</small></span>
                    </div>`;
                }).join('');
            }
        }

        // Modal Functions
        function showModal(modalId) {
            document.getElementById(modalId).classList.add('active');
        }

        function closeModal(modalId) {
            document.getElementById(modalId).classList.remove('active');
        }

        // Alert System
        function showAlert(message, type = 'success') {
            const alertsContainer = document.getElementById('alerts');
            const alert = document.createElement('div');
            alert.className = `alert alert-${type} fade-in`;
            alert.textContent = message;
            
            alertsContainer.appendChild(alert);
            
            setTimeout(() => {
                alert.remove();
            }, 5000);
        }

        // Initialize with some demo users
        function initializeDemo() {
            // Add demo admin user
            const adminUser = encryptDecrypt('admin');
            const adminPass = encryptDecrypt('admin123');
            users[adminUser] = adminPass;
            
            // Add demo regular user
            const demoUser = encryptDecrypt('demo');
            const demoPass = encryptDecrypt('demo123');
            users[demoUser] = demoPass;
            
            showAlert('Demo initialized! Try: admin/admin123 or demo/demo123', 'success');
        }

        // Initialize the application
        document.addEventListener('DOMContentLoaded', function() {
            initializeDemo();
            showLogin();
        });

        // Close modals when clicking outside
        window.onclick = function(event) {
            if (event.target.classList.contains('modal')) {
                event.target.classList.remove('active');
            }
        }
    </script>
</body>
</html>
