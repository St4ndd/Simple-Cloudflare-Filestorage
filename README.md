
# Simple-Cloudflare-Filestorage




# Cloudflare Worker File Storage Guide

This guide explains how to set up and deploy a Cloudflare Worker that interacts with Cloudflare R2 to handle file uploads, downloads, and deletions.

## 1. Create a Cloudflare Worker

1. **Log in to your Cloudflare account.**
   - Navigate to the Cloudflare dashboard.

2. **Go to the "Workers" section:**
   - From the sidebar, click on "Workers & Pages."
   - Click the "Create a Worker" button.

3. **Write the Worker Code:**
   - Copy and paste the following code into the Cloudflare Worker editor:

   ```javascript
   export default {
       async fetch(request, env) {
           try {
               const url = new URL(request.url);

               // 🟡 Upload File
               if (url.pathname.startsWith("/upload")) {
                   const formData = await request.formData();
                   const file = formData.get("file");

                   if (!file) {
                       return new Response("Error: File is missing", { status: 400 });
                   }

                   const fileName = file.name;

                   // Convert the Blob into an ArrayBuffer
                   const fileBuffer = await (await new Response(file)).arrayBuffer();

                   try {
                       // Store the file in the Cloudflare R2 Bucket
                       await env.R2_BUCKET.put(fileName, fileBuffer);
                       return new Response(`File '${fileName}' uploaded successfully.`, { status: 200 });
                   } catch (r2Error) {
                       console.log(`Error uploading the file: ${r2Error.message}`);
                       return new Response(`Error uploading the file: ${r2Error.message}`, { status: 500 });
                   }
               }

               // 🟢 Download File
               if (url.pathname.startsWith("/download/")) {
                   const fileName = url.pathname.split("/download/")[1];
                   try {
                       const object = await env.R2_BUCKET.get(fileName);

                       if (!object) {
                           return new Response("File not found.", { status: 404 });
                       }

                       const body = object.body;
                       return new Response(body, {
                           status: 200,
                           headers: {
                               "Content-Disposition": `attachment; filename="${fileName}"`,
                               "Content-Type": "application/octet-stream"
                           }
                       });
                   } catch (r2Error) {
                       console.log(`Error downloading the file: ${r2Error.message}`);
                       return new Response(`Error downloading the file: ${r2Error.message}`, { status: 500 });
                   }
               }

               // 🔴 Delete File
               if (url.pathname.startsWith("/delete/")) {
                   const fileName = url.pathname.split("/delete/")[1]; // Extract file name
                   try {
                       await env.R2_BUCKET.delete(fileName);
                       return new Response(`File '${fileName}' deleted successfully.`, { status: 200 });
                   } catch (r2Error) {
                       console.log(`Error deleting the file: ${r2Error.message}`);
                       return new Response(`Error deleting the file: ${r2Error.message}`, { status: 500 });
                   }
               }

               return new Response("Hello from Cloudflare R2!", { status: 200 });
           } catch (error) {
               console.log(`Error in worker: ${error.message}`);
               return new Response(`Error: ${error.message}`, { status: 500 });
           }
       }
   };
   ```

## 2. Configure the Cloudflare Worker with R2

1. **Set up an R2 Bucket:**
   - Follow the [Cloudflare R2 documentation](https://developers.cloudflare.com/r2/) to create an R2 bucket.

2. **Create a binding to your R2 bucket:**
   - In the Cloudflare Workers dashboard, under the "Environment" section, create a binding for your R2 bucket. Name the binding `R2_BUCKET`.

## 3. Deploy the Worker

1. **Deploy your worker:**
   - Once your worker is set up with the code, click "Save and Deploy."

2. **Access the Worker:**
   - Your worker will be accessible at `https://your-worker-name.workers.dev`.

## 4. Testing the Worker

- **Upload File:** `POST https://your-worker-name.workers.dev/upload`
- **Download File:** `GET https://your-worker-name.workers.dev/download/{fileName}`
- **Delete File:** `DELETE https://your-worker-name.workers.dev/delete/{fileName}`

That's it! You now have a fully functional Cloudflare Worker that handles file uploads, downloads, and deletions using Cloudflare R2.










# Various Guides for coding languages

## C# Guide

### 1️⃣ Initialize the Class
Create an instance of the `CloudflareFileStorage` class by passing the Cloudflare Worker URL:

```csharp
string cloudflareWorkerUrl = "https://your-cloudflare-worker-url.com";
var storage = new CloudflareFileStorage(cloudflareWorkerUrl);
```

### 2️⃣ Upload a File
Use the `UploadFileAsync` method to upload a file. Pass the full file path:

```csharp
string filePath = "C:\path\to\your\file.txt";
string uploadResponse = await storage.UploadFileAsync(filePath);
Console.WriteLine(uploadResponse);
```

### 3️⃣ Download a File
Use the `DownloadFileAsync` method to download a file. Pass the file name and the destination path:

```csharp
string fileName = "file.txt";
string savePath = "C:\path\to\save";
await storage.DownloadFileAsync(fileName, savePath);
Console.WriteLine($"File '{fileName}' downloaded successfully.");
```

### 4️⃣ Delete a File
Use the `DeleteFileAsync` method to delete a file. Pass the file name:

```csharp
string deleteResponse = await storage.DeleteFileAsync("file.txt");
Console.WriteLine(deleteResponse);
```

## Java Guide

### 1️⃣ Initialize the Class
Create an instance of the `CloudflareFileStorage` class:

```java
String cloudflareWorkerUrl = "https://your-cloudflare-worker-url.com";
CloudflareFileStorage storage = new CloudflareFileStorage(cloudflareWorkerUrl);
```

### 2️⃣ Upload a File
Use the `uploadFile` method to upload a file. Pass the full file path:

```java
String filePath = "C:\path\to\your\file.txt";
try {
    String uploadResponse = storage.uploadFile(filePath);
    System.out.println(uploadResponse);
} catch (IOException e) {
    e.printStackTrace();
}
```

### 3️⃣ Download a File
Use the `downloadFile` method to download a file. Pass the file name and the destination path:

```java
String fileName = "file.txt";
String savePath = "C:\path\to\save";
try {
    byte[] downloadedFile = storage.downloadFile(fileName, savePath);
    System.out.println("File '" + fileName + "' downloaded successfully.");
} catch (IOException e) {
    e.printStackTrace();
}
```

### 4️⃣ Delete a File
Use the `deleteFile` method to delete a file. Pass the file name:

```java
String deleteResponse;
try {
    deleteResponse = storage.deleteFile("file.txt");
    System.out.println(deleteResponse);
} catch (IOException e) {
    e.printStackTrace();
}
```

## C++ Guide

### 1️⃣ Initialize the Class
Create an instance of the `CloudflareFileStorage` class:

```cpp
std::string cloudflareWorkerUrl = "https://your-cloudflare-worker-url.com";
CloudflareFileStorage storage(cloudflareWorkerUrl);
```

### 2️⃣ Upload a File
Use the `uploadFile` method to upload a file:

```cpp
std::string filePath = "C:\path\to\your\file.txt";
try {
    std::string uploadResponse = storage.uploadFile(filePath);
    std::cout << uploadResponse << std::endl;
} catch (std::runtime_error &e) {
    std::cout << e.what() << std::endl;
}
```

### 3️⃣ Download a File
Use the `downloadFile` method to download a file:

```cpp
std::string fileName = "file.txt";
std::string savePath = "C:\path\to\save";
try {
    std::string downloadResponse = storage.downloadFile(fileName, savePath);
    std::cout << "File '" << fileName << "' downloaded successfully." << std::endl;
} catch (std::runtime_error &e) {
    std::cout << e.what() << std::endl;
}
```

### 4️⃣ Delete a File
Use the `deleteFile` method to delete a file:

```cpp
try {
    std::string deleteResponse = storage.deleteFile("file.txt");
    std::cout << deleteResponse << std::endl;
} catch (std::runtime_error &e) {
    std::cout << e.what() << std::endl;
}
```

## C Guide

### 1️⃣ Initialize the Class
Create an instance of the `CloudflareFileStorage` structure:

```c
CloudflareFileStorage *storage = create_cloudflare_storage("https://your-cloudflare-worker-url.com");
```

### 2️⃣ Upload a File
Use the `upload_file` function to upload a file:

```c
const char *filePath = "C:\path\to\your\file.txt";
char *uploadResponse = upload_file(storage, filePath);
printf("%s
", uploadResponse);
```

### 3️⃣ Download a File
Use the `download_file` function to download a file:

```c
const char *fileName = "file.txt";
const char *savePath = "C:\path\to\save";
char *downloadResponse = download_file(storage, fileName, savePath);
printf("%s
", downloadResponse);
```

### 4️⃣ Delete a File
Use the `delete_file` function to delete a file:

```c
char *deleteResponse = delete_file(storage, "file.txt");
printf("%s
", deleteResponse);
```

## TypeScript Guide

### 1️⃣ Initialize the Class
Create an instance of the `CloudflareFileStorage` class:

```typescript
const cloudflareWorkerUrl = "https://your-cloudflare-worker-url.com";
const storage = new CloudflareFileStorage(cloudflareWorkerUrl);
```

### 2️⃣ Upload a File
Use the `uploadFile` method to upload a file:

```typescript
const filePath = "C:\path\to\your\file.txt";
try {
    const uploadResponse = await storage.uploadFile(filePath);
    console.log(uploadResponse);
} catch (error) {
    console.error(error);
}
```

### 3️⃣ Download a File
Use the `downloadFile` method to download a file:

```typescript
const fileName = "file.txt";
const savePath = "C:\path\to\save";
try {
    const downloadedFile = await storage.downloadFile(fileName, savePath);
    console.log(`File '${fileName}' downloaded successfully.`);
} catch (error) {
    console.error(error);
}
```

### 4️⃣ Delete a File
Use the `deleteFile` method to delete a file:

```typescript
try {
    const deleteResponse = await storage.deleteFile("file.txt");
    console.log(deleteResponse);
} catch (error) {
    console.error(error);
}
```

## HTML (with JavaScript) Guide

### 1️⃣ Initialize the Class
Use JavaScript within HTML to interact with the Cloudflare Worker:

```html
<script>
    const cloudflareWorkerUrl = "https://your-cloudflare-worker-url.com";
</script>
```

### 2️⃣ Upload a File
Create an HTML form to upload a file:

```html
<input type="file" id="fileInput" />
<button onclick="uploadFile()">Upload File</button>

<script>
    async function uploadFile() {
        const fileInput = document.getElementById("fileInput");
        const file = fileInput.files[0];
        
        if (!file) {
            alert("Please select a file.");
            return;
        }

        const formData = new FormData();
        formData.append("file", file);

        try {
            const response = await fetch(`${cloudflareWorkerUrl}/upload`, {
                method: "POST",
                body: formData
            });
            const result = await response.text();
            alert("File uploaded successfully!");
            console.log(result);
        } catch (error) {
            console.error("Error uploading file:", error);
        }
    }
</script>
```

### 3️⃣ Download a File
Create a button to download a file:

```html
<button onclick="downloadFile()">Download File</button>

<script>
    async function downloadFile() {
        const fileName = "file.txt";
        try {
            const response = await fetch(`${cloudflareWorkerUrl}/download/${fileName}`);
            const fileBlob = await response.blob();
            const fileURL = URL.createObjectURL(fileBlob);
            const link = document.createElement("a");
            link.href = fileURL;
            link.download = fileName;
            link.click();
            alert("File downloaded successfully!");
        } catch (error) {
            console.error("Error downloading file:", error);
        }
    }
</script>
```

### 4️⃣ Delete a File
Create a button to delete a file:

```html
<button onclick="deleteFile()">Delete File</button>

<script>
    async function deleteFile() {
        const fileName = "file.txt"; // Example file name to delete
        try {
            const response = await fetch(`${cloudflareWorkerUrl}/delete/${fileName}`, {
                method: "DELETE"
            });
            const result = await response.text();
            alert("File deleted successfully!");
            console.log(result);
        } catch (error) {
            console.error("Error deleting file:", error);
        }
    }
</script>
```
