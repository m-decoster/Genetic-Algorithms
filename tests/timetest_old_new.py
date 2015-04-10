# Voer een tijdstest uit met telkens het gemiddelde van 5 runs
# om de oude implementatie met qsort te vergelijken met de nieuwe
# Voer uit met python3

import time
import subprocess

avgtime_new = 0
avgtime_old = 0

for j in range(0, 5):
    t0 = time.perf_counter()
    subprocess.call("./maxdist 5 vierhoek.txt", shell=True, stdout=subprocess.PIPE)
    avgtime_new += time.perf_counter() - t0

    t0 = time.perf_counter()
    subprocess.call("./maxdist_old 5 vierhoek.txt", shell=True, stdout=subprocess.PIPE)
    avgtime_old += time.perf_counter() - t0

avgtime_new /= 5
avgtime_old /= 5

print("nieuw: %f" % avgtime_new)
print("oud: %f" % avgtime_old)
