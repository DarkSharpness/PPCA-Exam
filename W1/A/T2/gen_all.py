import os

for i in range(1, 11):
    os.system(f'./std < data/{i}.in > data/{i}.out')