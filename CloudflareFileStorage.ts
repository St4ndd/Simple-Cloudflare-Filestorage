import * as fs from 'fs';
import axios from 'axios';

class CloudflareFileStorage {
  private workerUrl: string;

  constructor(workerUrl: string) {
    this.workerUrl = workerUrl.endsWith('/') ? workerUrl : `${workerUrl}/`;
  }

  async uploadFile(filePath: string): Promise<string> {
    if (!fs.existsSync(filePath)) {
      throw new Error(`File not found: ${filePath}`);
    }

    const formData = new FormData();
    formData.append('file', fs.createReadStream(filePath));

    const response = await axios.post(`${this.workerUrl}upload`, formData, {
      headers: {
        'Content-Type': 'application/octet-stream',
        ...formData.getHeaders(),
      },
    });

    return response.data;
  }

  async downloadFile(fileName: string, savePath: string): Promise<Buffer> {
    const response = await axios.get(`${this.workerUrl}download/${fileName}`, { responseType: 'arraybuffer' });
    const filePath = `${savePath}/${fileName}`;
    fs.writeFileSync(filePath, response.data);
    return response.data;
  }

  async deleteFile(fileName: string): Promise<string> {
    const response = await axios.delete(`${this.workerUrl}delete/${fileName}`);
    return response.data;
  }
}
