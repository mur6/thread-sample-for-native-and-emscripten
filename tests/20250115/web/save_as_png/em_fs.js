function showFileList(FS, directory) {
    const fileListDiv = document.getElementById('fileList');
    fileListDiv.innerHTML = ''; // Clear previous content

    try {
        const files = FS.readdir(directory);

        // Sort files alphabetically
        const sortedFiles = files
            .filter(file => file !== '.' && file !== '..')
            .sort((a, b) => a.localeCompare(b));

        for (const file of sortedFiles) {
            const fullPath = directory + '/' + file;
            const stat = FS.stat(fullPath);
            // ディレクトリの場合はスキップ
            if (FS.isDir(stat.mode)) continue;

            const fileDiv = document.createElement('div');
            fileDiv.className = 'file-item';
            fileDiv.textContent = file;
            fileDiv.onclick = () => downloadFile(FS, fullPath);
            fileListDiv.appendChild(fileDiv);
        }
    } catch (e) {
        console.error('Failed to read directory:', e);
        fileListDiv.innerHTML = '<div class="error">Error reading directory</div>';
    }
}

function downloadFile(FS, filepath) {
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
