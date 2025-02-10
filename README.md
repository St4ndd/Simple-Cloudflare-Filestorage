# Simple-Cloudflare-Filestorage


## C# application
1️⃣ Initialize the Class
csharp
Kopieren
Bearbeiten
string cloudflareWorkerUrl = "https://your-cloudflare-worker-url.com";
var storage = new CloudflareFileStorage(cloudflareWorkerUrl);
2️⃣ Upload a File
csharp
Kopieren
Bearbeiten
string filePath = "C:\\path\\to\\your\\file.txt";
string uploadResponse = await storage.UploadFileAsync(filePath);
Console.WriteLine(uploadResponse);
3️⃣ Download a File
csharp
Kopieren
Bearbeiten
string fileName = "file.txt";
string savePath = "C:\\path\\to\\save";
await storage.DownloadFileAsync(fileName, savePath);
Console.WriteLine($"File '{fileName}' downloaded successfully.");
4️⃣ Delete a File
csharp
Kopieren
Bearbeiten
string deleteResponse = await storage.DeleteFileAsync("file.txt");
Console.WriteLine(deleteResponse);
