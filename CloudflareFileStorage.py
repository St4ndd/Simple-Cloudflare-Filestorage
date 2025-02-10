import os
import requests

class CloudflareFileStorage:
    def __init__(self, worker_url):
        self.worker_url = worker_url.rstrip('/')
    
    def upload_file(self, file_path):
        if not os.path.exists(file_path):
            raise FileNotFoundError(f"File not found: {file_path}")

        with open(file_path, 'rb') as file:
            files = {'file': (os.path.basename(file_path), file, 'application/octet-stream')}
            response = requests.post(f"{self.worker_url}/upload", files=files)
            return response.text
    
    def download_file(self, file_name, save_path):
        response = requests.get(f"{self.worker_url}/download/{file_name}")
        if response.status_code != 200:
            raise Exception(f"Error downloading file: {response.reason}")

        file_path = os.path.join(save_path, file_name)
        with open(file_path, 'wb') as file:
            file.write(response.content)
        return response.content
    
    def delete_file(self, file_name):
        response = requests.delete(f"{self.worker_url}/delete/{file_name}")
        return response.text
