import requests
import time

def test_create_vote():
    json_data = {
        "vote_topic":"今天活动安排",
        "vote_items":["钓鱼","爬山","放风筝","户外烧烤"],
        "vote_creator_id":10,
        "vote_creator":"黄蓉",
        "vote_create_time":int(time.time()) * 1000 * 1000,
        "voters":[2,7,10],
        "vote_choice_type":0
    }
    url = "http://localhost:9000/api/v1/create_vote"
    resp = requests.post(url=url,json=json_data)
    print(resp)
    print(resp.json())

if __name__ == "__main__":
    test_create_vote()