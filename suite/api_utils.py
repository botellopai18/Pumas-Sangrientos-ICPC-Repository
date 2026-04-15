import time
import hashlib
import requests
import random
import string

BASE_URL = "https://codeforces.com/api/"

def make_cf_request(method, params, api_key, secret, retries=3):
    for i in range(retries):
        rand = str(random.randint(100000, 999999))
        params['apiKey'] = api_key
        params['time'] = int(time.time())
        sorted_params = "&".join([f"{k}={params[k]}" for k in sorted(params.keys())])
        sig_str = f"{rand}/{method}?{sorted_params}#{secret}"
        api_sig = rand + hashlib.sha512(sig_str.encode()).hexdigest()
        params['apiSig'] = api_sig

        try:
            time.sleep(1.5) 
            response = requests.get(BASE_URL + method, params=params, timeout=30)
            if response.status_code == 200:
                return response.json()
            if response.status_code == 503:
                time.sleep(5 * (i + 1))
                continue
            return response.json()
        except:
            time.sleep(2)
    return None

def get_problem_list(last_letter):
    idx = string.ascii_uppercase.index(last_letter.upper())
    return list(string.ascii_uppercase[:idx+1])
