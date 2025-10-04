import requests
import time
# import random


def test_create_vote():
    json_data = {
        "vote_topic":f"今天活动安排{int(time.time())}",
        "vote_items":["钓鱼","爬山","放风筝","户外烧烤"],
        "vote_creator_id":10,
        "vote_creator":"黄蓉",
        "vote_create_time":int(time.time()) * 1000 * 1000,
        "voters":[2,7,10],
        # "voter_names":["李娜","孙浩","黄蓉"],
        "vote_choice_type":0
    }
    url = "http://localhost:9000/api/v1/create_vote"
    resp = requests.post(url=url,json=json_data)
    print(resp)
    print(resp.json())

def test_get_participate_vote_data():
    json_data = {
        "voter_id":10,
        "vote_num":4,
        "vote_offset":0,
        "vote_status":[0,1,2],
        "vote_process_status":[0,1]
    }
    url = "http://localhost:9000/api/v1/get_chunk_participate_vote_data"
    resp = requests.post(url,json=json_data) # tell it we use application/json,fuck!
    print(resp.json())

def test_get_participate_vote_num():
    json_data = {
        "voter_id":10,
        "vote_status":[0,1,2],
        "vote_process_status":[0,1]
    }  
    url = "http://localhost:9000/api/v1/get_participate_vote_num"
    resp = requests.post(url,json=json_data) # tell it we use application/json,fuck!
    print(resp.json())

if __name__ == "__main__":
    test_create_vote()
    # test_get_participate_vote_data()
    # test_get_participate_vote_num()