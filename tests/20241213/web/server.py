import argparse
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
    # set the directory to serve files from
    class MyHandler(WasmHandler):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, directory=directory, **kwargs)

    httpd = socketserver.TCPServer(("", port), MyHandler)
    with httpd:
        print(f"Serve files from directory {directory}")
        print(f"Listening on port {port}. Press Ctrl+C to stop.")
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            pass
        print("Server stopped.")
        httpd.server_close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--port", type=int, default=8088)
    parser.add_argument("--directory", type=str, default=".")
    args = parser.parse_args()
    run_server(args.port, args.directory)
