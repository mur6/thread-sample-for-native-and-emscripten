
function showFileList(currentPath = '/') {
    const fileListDiv = document.getElementById('fileList');
    fileListDiv.innerHTML = ''; // Clear previous content

    try {
        const files = FS.readdir(currentPath);

        // Current path display
        if (currentPath !== '/') {
            const parentPath = currentPath.split('/').slice(0, -2).join('/') + '/';
            const upLink = document.createElement('div');
            upLink.className = 'file-item directory';
            upLink.textContent = '../';
            upLink.onclick = () => showFileList(parentPath);
            fileListDiv.appendChild(upLink);
        }

        // Sort files: directories first, then regular files
        const sortedFiles = files.sort((a, b) => {
            if (a === '.' || a === '..') return -1;
            if (b === '.' || b === '..') return 1;

            const statA = FS.stat(currentPath + a);
            const statB = FS.stat(currentPath + b);
            const isDirectoryA = FS.isDir(statA.mode);
            const isDirectoryB = FS.isDir(statB.mode);

            if (isDirectoryA && !isDirectoryB) return -1;
            if (!isDirectoryA && isDirectoryB) return 1;
            return a.localeCompare(b);
        });

        for (const file of sortedFiles) {
            if (file === '.' || file === '..') continue;

            const fullPath = currentPath + file;
            const stat = FS.stat(fullPath);
            const isDirectory = FS.isDir(stat.mode);

            const fileDiv = document.createElement('div');
            fileDiv.className = `file-item ${isDirectory ? 'directory' : 'file'}`;
            fileDiv.textContent = file + (isDirectory ? '/' : '');

            if (isDirectory) {
                fileDiv.onclick = () => showFileList(fullPath + '/');
            } else {
                fileDiv.onclick = () => downloadFile(fullPath);
            }

            fileListDiv.appendChild(fileDiv);
        }
    } catch (e) {
        console.error('Failed to read directory:', e);
        fileListDiv.innerHTML = '<div class="error">Error reading directory</div>';
    }
}

function downloadFile(filepath) {
    try {
        // ファイルの内容を読み取り
        const content = FS.readFile(filepath);

        // Blobを作成
        const blob = new Blob([content], { type: 'application/octet-stream' });

        // ダウンロードリンクを作成
        const url = window.URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = filepath.split('/').pop(); // パスからファイル名を抽出

        // リンクをクリックしてダウンロードを開始
        document.body.appendChild(a);
        a.click();

        // クリーンアップ
        window.URL.revokeObjectURL(url);
        document.body.removeChild(a);
    } catch (e) {
        console.error('Failed to download file:', e);
        alert('Failed to download file: ' + e.message);
    }
}


export { showFileList, downloadFile };
