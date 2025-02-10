import java.io.*;
import java.net.*;
import java.nio.file.*;
import java.util.*;

public class CloudflareFileStorage {
    private final String workerUrl;

    public CloudflareFileStorage(String workerUrl) {
        this.workerUrl = workerUrl.endsWith("/") ? workerUrl : workerUrl + "/";
    }

    public String uploadFile(String filePath) throws IOException {
        File file = new File(filePath);
        if (!file.exists()) {
            throw new FileNotFoundException("File not found: " + filePath);
        }

        URL url = new URL(workerUrl + "upload");
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setDoOutput(true);
        connection.setRequestMethod("POST");
        connection.setRequestProperty("Content-Type", "application/octet-stream");

        try (OutputStream os = connection.getOutputStream(); FileInputStream fis = new FileInputStream(file)) {
            byte[] buffer = new byte[1024];
            int bytesRead;
            while ((bytesRead = fis.read(buffer)) != -1) {
                os.write(buffer, 0, bytesRead);
            }
        }

        try (BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()))) {
            return in.readLine();
        }
    }

    public byte[] downloadFile(String fileName, String savePath) throws IOException {
        URL url = new URL(workerUrl + "download/" + fileName);
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod("GET");

        try (InputStream in = connection.getInputStream()) {
            Path path = Paths.get(savePath, fileName);
            Files.copy(in, path, StandardCopyOption.REPLACE_EXISTING);
            return Files.readAllBytes(path);
        }
    }

    public String deleteFile(String fileName) throws IOException {
        URL url = new URL(workerUrl + "delete/" + fileName);
        HttpURLConnection connection = (HttpURLConnection) url.openConnection();
        connection.setRequestMethod("DELETE");

        try (BufferedReader in = new BufferedReader(new InputStreamReader(connection.getInputStream()))) {
            return in.readLine();
        }
    }
}
