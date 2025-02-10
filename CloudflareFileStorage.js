const fs = require('fs');
const axios = require('axios');

class CloudflareFileStorage {
    constructor(workerUrl) {
        this.workerUrl = workerUrl.replace(/\/$/, '');
    }

    async uploadFile(filePath) {
        if (!fs.existsSync(filePath)) {
            throw new Error(`File not found: ${filePath}`);
        }

        const fileContent = fs.createReadStream(filePath);
        const formData = new FormData();
        formData.append('file', fileContent, { filename: path.basename(filePath) });

        const response = await axios.post(`${this.workerUrl}/upload`, formData, {
            headers: {
                ...formData.getHeaders(),
                'Content-Type': 'application/octet-stream',
            },
        });

        return response.data;
    }

    async downloadFile(fileName, savePath) {
        const response = await axios.get(`${this.workerUrl}/download/${fileName}`, { responseType: 'arraybuffer' });
        if (response.status !== 200) {
            throw new Error(`Error downloading file: ${response.statusText}`);
        }

        const filePath = path.join(savePath, fileName);
        fs.writeFileSync(filePath, response.data);
        return response.data;
    }

    async deleteFile(fileName) {
        const response = await axios.delete(`${this.workerUrl}/delete/${fileName}`);
        return response.data;
    }
}
