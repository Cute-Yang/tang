import websocket
import json

def on_message(ws,message):
    print(f"Received:{message}")
    try:
        msg = json.loads(message)
        print(f"get a json data {msg}")
    except Exception as ex:
        pass

def on_error(ws,error):
    print(f"Error:{error}")

def on_close(ws,p1,p2):
    print("Connection closed")

def on_open(ws:websocket.WebSocketApp):
    print("Connection established!")
    ws.send("Hello,server!")

if __name__ == "__main__":
    ws:websocket.WebSocketApp = websocket.WebSocketApp(
        # 这里可以附带参数,哈哈
        "ws://localhost:9000/chat?user_id=9&user_name=戈薇",
        on_message=on_message,
        on_error=on_error,
        on_close=on_close
    )
    
    ws.on_open = on_open
    ws.run_forever()