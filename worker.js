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

                // Umwandlung des Blob in ArrayBuffer
                const fileBuffer = await (await new Response(file)).arrayBuffer();

                try {
                    // Datei im Cloudflare R2 Bucket speichern
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
                const fileName = url.pathname.split("/delete/")[1]; // Extrahiere den Dateinamen
                try {
                    await env.R2_BUCKET.delete(fileName);
                    return new Response(`File '${fileName}' deleted successfully.`, { status: 200 });
                } catch (r2Error) {
                    console.log(`Error deleting the file: ${r2Error.message}`);
                    return new Response(Error deleting the file: ${r2Error.message}`, { status: 500 });
                }
            }

            return new Response("Hello from Cloudflare R2!", { status: 200 });
        } catch (error) {
            console.log(`Error in worker: ${error.message}`);
            return new Response(`Error: ${error.message}`, { status: 500 });
        }
    }
};
