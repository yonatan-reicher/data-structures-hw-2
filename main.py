from dataclasses import dataclass
from typing import Callable, List, Union
from random import randint, choice



@dataclass
class Command:
    arity: int
    function: Callable

@dataclass
class Player:
    strength: int

@dataclass
class Team:
    id: int
    wins: int
    players: List[Player]

    def strength(self):
        if not self.players:
            return 0
        median_player = sorted(self.players, key=lambda player: player.strength)[len(self.players) // 2]
        return median_player.strength * len(self.players)

    def rank(self):
        # Edge cases
        if len(self.players) == 0:
            return 0
        return self.strength() + self.wins

@dataclass
class State:
    teams: List[Team]

    def team_exists(self, team_id: int):
        return any(team.id == team_id for team in self.teams)

    def team(self, team_id: int):
        return next(team for team in self.teams if team.id == team_id)

@dataclass
class Output:
    status: str
    value: int

SUCCESS = 'SUCCESS'
INVALID_INPUT = 'INVALID_INPUT'
FAILURE = 'FAILURE'


def add_team(state: State, team_id: int):
    if team_id <= 0:
        return INVALID_INPUT
    if state.team_exists(team_id):
        return FAILURE
    state.teams.append(Team(team_id, 0, []))
    return SUCCESS

def remove_team(state: State, team_id: int):
    if team_id <= 0:
        return INVALID_INPUT
    if not state.team_exists(team_id):
        return FAILURE
    state.teams = [team for team in state.teams if team.id != team_id]
    return SUCCESS

def add_player(state: State, team_id: int, player_strength: int):
    if player_strength <= 0 or team_id <= 0:
        return INVALID_INPUT
    if not state.team_exists(team_id):
        return FAILURE
    team = state.team(team_id)
    team.players.append(Player(player_strength))
    return SUCCESS

def remove_newest_player(state: State, team_id: int):
    if team_id <= 0: return INVALID_INPUT
    if not state.team_exists(team_id): return FAILURE
    if len(state.team(team_id).players) == 0: return FAILURE

    team = state.team(team_id)
    team.players.pop()
    return SUCCESS

def play_match(state: State, team_id1: int, team_id2: int):
    if team_id1 <= 0 or team_id2 <= 0 or team_id1 == team_id2:
        return Output(INVALID_INPUT, 0)
    if not state.team_exists(team_id1) or not state.team_exists(team_id2):
        return Output(FAILURE, 0)

    team1 = state.team(team_id1)
    team2 = state.team(team_id2)
    winner = team1.id
    if team1.strength() > team2.strength():
        team1.wins += 1
    elif team2.strength() > team1.strength():
        team2.wins += 1
        winner = team2.id
    elif team1.id < team2.id:
        team1.wins += 1
    else:
        team2.wins += 1
        winner = team2.id
    return Output(SUCCESS, winner)

def num_wins_for_team(state: State, team_id: int):
    if team_id <= 0: return Output(INVALID_INPUT, 0)
    if not state.team_exists(team_id): return Output(FAILURE, 0)
    team = state.team(team_id)
    return Output(SUCCESS, team.wins)

# This should actually return the highest rank, not the team itself
def get_highest_ranked_team(state: State):
    if not state.teams: return Output(SUCCESS, -1)
    r = max(state.teams, key=lambda team: team.rank()).rank()
    return Output(SUCCESS, r)

def unite_teams(state: State, team_id1: int, team_id2: int):
    if team_id1 <= 0 or team_id2 <= 0 or team_id1 == team_id2:
        return INVALID_INPUT
    if not state.team_exists(team_id1) or not state.team_exists(team_id2):
        return FAILURE
    team1 = state.team(team_id1)
    team2 = state.team(team_id2)
    team1.players.extend(team2.players)
    remove_team(state, team_id2)
    return SUCCESS

def play_tournament(state: State, low_power: int, high_power: int):
    if low_power <= 0 or high_power <= 0 or low_power > high_power:
        return INVALID_INPUT
    teams = [team for team in state.teams if low_power <= team.strength() <= high_power]
    if len(teams).bit_count() != 1:
        return FAILURE
    while len(teams) > 1:
        next_teams = []
        half = len(teams) // 2
        for i in range(0, half):
            winner = play_match(state, teams[i].id, teams[half + i].id).value
            next_teams.append(state.team(winner))
    return teams[0].id if teams else None

commands = {
    'add_team': Command(1, add_team),
    'remove_team': Command(1, remove_team),
    'add_player': Command(2, add_player),
    'remove_newest_player': Command(1, remove_newest_player),
    'play_match': Command(2, play_match),
    'num_wins_for_team': Command(1, num_wins_for_team),
    'get_highest_ranked_team': Command(0, get_highest_ranked_team),
    'unite_teams': Command(2, unite_teams),
    'play_tournament': Command(2, play_tournament),
}


def run_exe(stdin: str) -> str:
    import subprocess
    import sys
    cmd = ['main.exe']
    p = subprocess.run(cmd, capture_output=True, input=stdin.encode())
    return p.stdout.decode().strip()


def generate_command():
    ARG_MIN = -5
    ARG_MAX = 10
    command_key = choice(['add_team'] + list(commands.keys()))
    command = commands[command_key]
    args = [ randint(ARG_MIN, ARG_MAX) for _ in range(command.arity) ]
    return f'{command_key} {" ".join(map(str, args))}'

def generate_input():
    N = randint(1, 20)
    return '\n'.join([generate_command() for _ in range(N)])

def run_command(state: State, line: str) -> str:
    command_name = line.split()[0]
    args = list(map(int, line.split()[1:]))
    command = commands[command_name]
    out = command.function(state, *args)
    if isinstance(out, Output):
        if out.status == SUCCESS:
            return f'{command_name}: {out.status}, {out.value}'
        else:
            return f'{command_name}: {out.status}'
    else:
        return f'{command_name}: {out}'

def run_test(inp: str) -> str:
    state = State([])
    output = []
    for line in inp.splitlines():
        output.append(run_command(state, line))
    return '\n'.join(output)

inp = generate_input()
with open('last_input.txt', 'w') as f:
    f.write(inp)

py_out = run_test(inp).replace('\r\n', '\n') + '\n'
cpp_out = run_exe(inp).replace('\r\n', '\n') + '\n'

import difflib

print('Equal? ', py_out == cpp_out)
d = difflib.ndiff(
    py_out.splitlines(keepends=True),
    cpp_out.splitlines(keepends=True),
)
print(''.join(d))
exit()
for (i, linediff) in enumerate(d):
    print(f'At line {i + 1}:')
    print(linediff)

