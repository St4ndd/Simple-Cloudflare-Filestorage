using System;
using System.IO;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Threading.Tasks;

public class CloudflareFileStorage
{
    private readonly HttpClient _httpClient;
    private readonly string _workerUrl; // Cloudflare Worker URL

    public CloudflareFileStorage(string workerUrl)
    {
        _httpClient = new HttpClient();
        _workerUrl = workerUrl.TrimEnd('/');
    }

    public async Task<string> UploadFileAsync(string filePath)
    {
        if (!File.Exists(filePath))
            throw new FileNotFoundException("File not found.", filePath);

        using var content = new MultipartFormDataContent();
        using var fileStream = new FileStream(filePath, FileMode.Open, FileAccess.Read);
        var fileContent = new StreamContent(fileStream);
        fileContent.Headers.ContentType = MediaTypeHeaderValue.Parse("application/octet-stream");
        content.Add(fileContent, "file", Path.GetFileName(filePath));

        var response = await _httpClient.PostAsync($"{_workerUrl}/upload", content);
        return await response.Content.ReadAsStringAsync();
    }

    public async Task<byte[]> DownloadFileAsync(string fileName, string savePath)
    {
        var response = await _httpClient.GetAsync($"{_workerUrl}/download/{fileName}");
        if (!response.IsSuccessStatusCode)
            throw new Exception($"Error downloading file: {response.ReasonPhrase}");

        var fileBytes = await response.Content.ReadAsByteArrayAsync();
        await File.WriteAllBytesAsync(Path.Combine(savePath, fileName), fileBytes);
        return fileBytes;
    }

    public async Task<string> DeleteFileAsync(string fileName)
    {
        var response = await _httpClient.DeleteAsync($"{_workerUrl}/delete/{fileName}");
        return await response.Content.ReadAsStringAsync();
    }
}
