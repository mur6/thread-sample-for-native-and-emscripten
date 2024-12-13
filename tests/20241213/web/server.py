import socketserver
from http.server import SimpleHTTPRequestHandler


class WasmHandler(SimpleHTTPRequestHandler):
    def end_headers(self):
        # Cross-Origin-Embedder-Policyヘッダの設定
        self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
        # Cross-Origin-Opener-Policyヘッダの設定
        self.send_header("Cross-Origin-Opener-Policy", "same-origin")
        SimpleHTTPRequestHandler.end_headers(self)


# server http server on port 8000, given directory by argument
def run_server(port, directory):
    httpd = socketserver.TCPServer(("", port), WasmHandler)
    httpd.serve_forever()
    httpd.directory = directory
    return httpd


if __name__ == "__main__":
    PORT = 8080
    with run_server(PORT, ".") as httpd:
        print("Listening on port {}. Press Ctrl+C to stop.".format(PORT))
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            pass
        httpd.server_close()
