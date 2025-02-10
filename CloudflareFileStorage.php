<?php

class CloudflareFileStorage
{
    private $workerUrl;

    public function __construct($workerUrl)
    {
        $this->workerUrl = rtrim($workerUrl, '/');
    }

    public function uploadFile($filePath)
    {
        if (!file_exists($filePath)) {
            throw new Exception("File not found: $filePath");
        }

        $file = new CURLFile($filePath, 'application/octet-stream', basename($filePath));
        $data = ['file' => $file];
        $ch = curl_init("{$this->workerUrl}/upload");
        curl_setopt($ch, CURLOPT_POST, true);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $data);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        $response = curl_exec($ch);
        curl_close($ch);

        return $response;
    }

    public function downloadFile($fileName, $savePath)
    {
        $ch = curl_init("{$this->workerUrl}/download/$fileName");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        $fileContent = curl_exec($ch);
        curl_close($ch);

        if ($fileContent === false) {
            throw new Exception("Error downloading file.");
        }

        file_put_contents($savePath . DIRECTORY_SEPARATOR . $fileName, $fileContent);
        return $fileContent;
    }

    public function deleteFile($fileName)
    {
        $ch = curl_init("{$this->workerUrl}/delete/$fileName");
        curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        $response = curl_exec($ch);
        curl_close($ch);

        return $response;
    }
}
?>
