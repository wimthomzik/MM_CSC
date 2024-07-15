import generator as g
import tester
import numpy as np

examples = []

for i in range(100):
        examples.append(g.generate(1000,1000,0.03,True,np.random.randint(0,1000000)))
i = 0
for ex in examples:
    for ex2 in examples:
        if not tester.run(ex,ex2,True):
            print("Error multiplying: " + ex + " and " + ex2)
        print(f"#{i} passed")
        i+=1