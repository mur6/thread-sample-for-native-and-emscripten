import socketserver
from http.server import SimpleHTTPRequestHandler


class WasmHandler(SimpleHTTPRequestHandler):
    def end_headers(self):
        # Cross-Origin-Embedder-Policyヘッダの設定
        self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
        # Cross-Origin-Opener-Policyヘッダの設定
        self.send_header("Cross-Origin-Opener-Policy", "same-origin")
        SimpleHTTPRequestHandler.end_headers(self)


if __name__ == "__main__":
    PORT = 8080
    with socketserver.TCPServer(("", PORT), WasmHandler) as httpd:
        print("Listening on port {}. Press Ctrl+C to stop.".format(PORT))
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            pass
        httpd.server_close()
