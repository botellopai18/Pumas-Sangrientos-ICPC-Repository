from api_utils import make_cf_request, get_problem_list

def fetch_team_data(api_key, secret, group_code, handles, contests_config):
    target_handles = [h.lower() for h in handles]
    user_stats = {h.lower(): {"contest": set(), "upsolve": set()} for h in handles}
    contest_reports = []
    all_solved_problems = set()
    total_problems_in_gym = 0

    c_list_resp = make_cf_request("contest.list", {"gym": "true", "groupCode": group_code}, api_key, secret)
    if not c_list_resp or c_list_resp.get('status') != 'OK':
        return None

    contests_meta = {str(c['id']): c for c in c_list_resp['result']}

    for c_id, last_p in contests_config.items():
        if c_id not in contests_meta: continue
        
        problems_range = get_problem_list(last_p)
        total_problems_in_gym += len(problems_range)
        contest = contests_meta[c_id]
        start_time = contest.get('startTimeSeconds', 0)
        end_time = start_time + contest.get('durationSeconds', 0)

        status_resp = make_cf_request("contest.status", {"contestId": c_id, "asManager": "true"}, api_key, secret)
        
        solved_in_c = set()
        solved_up = set()

        if status_resp and status_resp.get('status') == 'OK':
            for sub in status_resp['result']:
                if sub.get('verdict') != 'OK': continue
                p_idx = sub['problem']['index'].upper()
                if p_idx not in problems_range: continue
                
                authors = [m['handle'].lower() for m in sub['author']['members']]
                for auth in authors:
                    if auth in target_handles:
                        p_unique = (c_id, p_idx)
                        if start_time > 0 and sub['creationTimeSeconds'] <= end_time:
                            solved_in_c.add(p_idx)
                            user_stats[auth]["contest"].add(p_unique)
                            all_solved_problems.add(p_unique)
                        else:
                            solved_up.add(p_idx)
                            user_stats[auth]["upsolve"].add(p_unique)
                            all_solved_problems.add(p_unique)

        solved_up -= solved_in_c
        missing = [p for p in problems_range if p not in solved_in_c and p not in solved_up]
        
        contest_reports.append({
            "name": contest['name'],
            "id": c_id,
            "solved": sorted(list(solved_in_c)),
            "upsolved": sorted(list(solved_up)),
            "missing": sorted(missing)
        })

    return {
        "user_stats": user_stats,
        "contest_reports": contest_reports,
        "total_gym_problems": total_problems_in_gym,
        "total_solved_unique": len(all_solved_problems)
    }