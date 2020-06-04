import os
import numpy as np

def makeseed(myin):
    return myin

for als in [172.5,175.0]:
    for j in range(400):
        
        myname = str(als).replace(".","_")

        myfile = open("run_"+myname+"_"+str(j)+".sh","w")
        myfile.write("#!/bin/bash\n")
        myfile.write("source setup.sh\n")
        
        os.system("cp examples/Pythia8/configNoLHE_more.cmnd examples/Pythia8/config_"+str(j)+"_"+myname+".cmnd")
        os.system("sed -i -e 's/6:m0 = 175/6:m0 = "+str(als)+"/g' examples/Pythia8/config_"+str(j)+"_"+myname+".cmnd")
        os.system("sed -i -e 's/Random:seed = -1/Random:seed = "+str(makeseed(j))+"/g' examples/Pythia8/config_"+str(j)+"_"+myname+".cmnd")
        myfile.write("./DelphesPythia8 cards/delphes_card_CMS.tcl examples/Pythia8/config_"+str(j)+"_"+myname+".cmnd delphes_nolhe"+str(j)+"_"+myname+".root\n")
        #myfile.write("root -b -x -q 'examples/topmass.C(\"delphes_nolhe"+str(j)+"_"+str(als)+".root\",\"output_det_"+str(j)+"_"+str(als)+".txt\",\"output_part_"+str(j)+"_"+str(als)+".txt\")'") 

        myfile.write("pwd\n")
        myfile.write("ls /scratch\n")
        myfile.write("mkdir /scratch/bpn7\n")
        myfile.write("root -b -x -q 'examples/topmass.C(\"delphes_nolhe"+str(j)+"_"+myname+".root\",\"/scratch/bpn7/output_det_"+str(j)+"_"+myname+".txt\",\"/scratch/bpn7/output_part_"+str(j)+"_"+myname+".txt\","+str(als)+")'\n")
        myfile.write("ls /scratch/bpn7\n")
        myfile.write("cp /scratch/bpn7/output_det_"+str(j)+"_"+myname+".txt .\n")
        myfile.write("cp /scratch/bpn7/output_part_"+str(j)+"_"+myname+".txt .\n")
        myfile.write("rm delphes_nolhe"+str(j)+"_"+myname+".root\n")
        myfile.write("rm /scratch/bpn7/output_det_"+str(j)+"_"+myname+".txt\n")
        myfile.write("rm /scratch/bpn7/output_part_"+str(j)+"_"+myname+".txt\n")
        
        os.system("chmod +x run_"+myname+"_"+str(j)+".sh") 
        os.system("bsub -o run_"+myname+"_"+str(j)+".log run_"+myname+"_"+str(j)+".sh")
