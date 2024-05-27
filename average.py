import os
import pandas as pd
directory = os.getcwd()
files = sorted([f for f in os.listdir(directory) if (f.endswith('.csv') and "avera" not in f)])
file_groups = {}
for f in files:
    prefix = f[:-7]
    if prefix not in file_groups:
        file_groups[prefix] = []
    file_groups[prefix].append(f)
for group in file_groups.items():
    dfs = []
    prefix = group[0]
    for file in group[1]:
        df = pd.read_csv(os.path.join(directory, file),sep="\t", header=None)
        df = df.drop_duplicates(subset=[1], keep='first')
        dfs.append(df)

    dfResult = dfs[0]
    for i in range(1,len(dfs)):
        dfResult = dfResult.add(dfs[i])
    (dfResult/len(group[1])).to_csv(os.path.join(directory,
                                                    f'{prefix}_averaged.csv'),
                                                    index=False, header=False, sep="\t")
