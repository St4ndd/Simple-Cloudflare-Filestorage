package main

import (
    "fmt"
    "io/ioutil"
    "mime/multipart"
    "net/http"
    "os"
    "path/filepath"
)

type CloudflareFileStorage struct {
    WorkerURL string
}

func (c *CloudflareFileStorage) UploadFile(filePath string) (string, error) {
    file, err := os.Open(filePath)
    if err != nil {
        return "", err
    }
    defer file.Close()

    body := &multipart.Writer{}
    req, err := http.NewRequest("POST", fmt.Sprintf("%s/upload", c.WorkerURL), body)
    req.Header.Set("Content-Type", body.FormDataContentType())
    req.MultipartForm = map[string][]string{"file": {file.Name()}}

    client := &http.Client{}
    res, err := client.Do(req)
    if err != nil {
        return "", err
    }
    defer res.Body.Close()

    result, err := ioutil.ReadAll(res.Body)
    return string(result), err
}

func (c *CloudflareFileStorage) DownloadFile(fileName, savePath string) ([]byte, error) {
    res, err := http.Get(fmt.Sprintf("%s/download/%s", c.WorkerURL, fileName))
    if err != nil {
        return nil, err
    }
    defer res.Body.Close()

    body, err := ioutil.ReadAll(res.Body)
    if err != nil {
        return nil, err
    }

    saveFile := filepath.Join(savePath, fileName)
    err = ioutil.WriteFile(saveFile, body, 0644)
    return body, err
}

func (c *CloudflareFileStorage) DeleteFile(fileName string) (string, error) {
    req, err := http.NewRequest("DELETE", fmt.Sprintf("%s/delete/%s", c.WorkerURL, fileName), nil)
    if err != nil {
        return "", err
    }

    client := &http.Client{}
    res, err := client.Do(req)
    if err != nil {
        return "", err
    }
    defer res.Body.Close()

    result, err := ioutil.ReadAll(res.Body)
    return string(result), err
}
