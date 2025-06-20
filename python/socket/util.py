from datetime import datetime

def log_i(tag, str):
    str_curtime = datetime.now().strftime("%H:%M:%S")
    print(f"[{str_curtime}][{tag}][I]{str}")