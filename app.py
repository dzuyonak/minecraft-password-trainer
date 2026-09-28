import os
import sys
import json
import configparser
import threading
import webbrowser
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer

APP_DIR = os.path.dirname(os.path.abspath(__file__))
CONFIG_FILE = os.path.join(APP_DIR, "config.ini")
PORT = 8765

def read_config():
    config = configparser.ConfigParser()
    if not os.path.exists(CONFIG_FILE):
        config['Credentials'] = {'login': 'steve', 'password': 'diamond'}
        config['Options'] = {'language': 'ru', 'show_password': '1', 'show_hint': '1'}
        with open(CONFIG_FILE, 'w', encoding='utf-8') as f:
            f.write("# Настройки для тренировки логина и пароля\n")
            config.write(f)
        return {'login': 'steve', 'password': 'diamond', 'language': 'ru', 'show_password': 1, 'show_hint': 1}

    try:
        config.read(CONFIG_FILE, encoding='utf-8')
        login = config.get('Credentials', 'login', fallback='steve').strip()
        password = config.get('Credentials', 'password', fallback='diamond').strip()
        language = config.get('Options', 'language', fallback='ru').strip().lower()
        if language not in ['ru', 'en', 'pl']:
            language = 'ru'
        show_password = config.getint('Options', 'show_password', fallback=1)
        show_hint = config.getint('Options', 'show_hint', fallback=1)
        return {
            'login': login,
            'password': password,
            'language': language,
            'show_password': show_password,
            'show_hint': show_hint
        }
    except Exception:
        return {'login': 'steve', 'password': 'diamond', 'language': 'ru', 'show_password': 1, 'show_hint': 1}

def write_config(login, password, show_hint=1, language='ru', show_password=1):
    config = configparser.ConfigParser()
    config['Credentials'] = {'login': login.strip(), 'password': password.strip()}
    config['Options'] = {
        'language': language.strip().lower(),
        'show_password': str(show_password),
        'show_hint': str(show_hint)
    }
    with open(CONFIG_FILE, 'w', encoding='utf-8') as f:
        f.write("# Настройки для тренировки логина и пароля\n")
        config.write(f)

class MinecraftHandler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=APP_DIR, **kwargs)

    def do_GET(self):
        if self.path == '/api/config':
            cfg = read_config()
            data = json.dumps(cfg).encode('utf-8')
            self.send_response(200)
            self.send_header('Content-Type', 'application/json; charset=utf-8')
            self.send_header('Content-Length', str(len(data)))
            self.send_header('Cache-Control', 'no-cache, no-store')
            self.end_headers()
            self.wfile.write(data)
            return

        if self.path == '/':
            self.path = '/index.html'

        return super().do_GET()

    def do_POST(self):
        if self.path == '/api/config':
            content_length = int(self.headers.get('Content-Length', 0))
            body = self.rfile.read(content_length).decode('utf-8')
            try:
                data = json.loads(body)
                login = data.get('login', '').strip()
                password = data.get('password', '').strip()
                show_hint = data.get('show_hint', 1)
                language = data.get('language', 'ru')
                show_password = data.get('show_password', 1)
                if login and password:
                    write_config(login, password, show_hint, language, show_password)
                    resp = json.dumps({'status': 'ok'}).encode('utf-8')
                    self.send_response(200)
                    self.send_header('Content-Type', 'application/json')
                    self.end_headers()
                    self.wfile.write(resp)
                    return
            except Exception as e:
                pass
            self.send_response(400)
            self.end_headers()
            return

        return super().do_POST()

    def log_message(self, format, *args):
        pass

def main():
    server_address = ('127.0.0.1', PORT)
    try:
        httpd = ThreadingHTTPServer(server_address, MinecraftHandler)
    except OSError:
        server_address = ('127.0.0.1', PORT + 1)
        httpd = ThreadingHTTPServer(server_address, MinecraftHandler)

    url = f"http://{server_address[0]}:{server_address[1]}"
    print("=" * 60)
    print("🎮 MINECRAFT: ТРЕНАЖЁР ЛОГИНА И ПАРОЛЯ")
    print("🌍 ЯЗЫКИ: Русский (RU) | English (EN) | Polski (PL)")
    print("=" * 60)
    print(f"🚀 Приложение запущено: {url}")
    print("📂 Синхронизировано с файлом: config.ini")
    print("⏹ Для выхода нажмите Ctrl + C в этом терминале.")
    print("=" * 60)

    # Automatically open browser window
    threading.Timer(0.3, lambda: webbrowser.open(url)).start()

    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\nПриложение остановлено.")
        httpd.server_close()

if __name__ == "__main__":
    main()
