# Voer een tijdstest uit met telkens het gemiddelde van 5 runs
# voor volgende aantallen punten:
# 5, 55, 105, 155, 205
# Voer uit met python3

import time
import subprocess

for i in range(5, 255, 50):
    avgtime = 0
    for j in range(0, 5):
        t0 = time.perf_counter()
        subprocess.call("./maxdist %d vierhoek.txt" % i, shell=True, stdout=subprocess.PIPE)
        avgtime += time.perf_counter() - t0
    avgtime /= 5
    print("%d punten: %f" % (i, avgtime))
