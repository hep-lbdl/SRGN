import os
import numpy as np

def makeseed(myin):
    return myin

for i in range(500):
        
    myfile = open("run_"+str(i)+".sh","w")
    myfile.write("#!/bin/bash\n")
    myfile.write("source setup.sh\n")
    
    myfiles_det = []
    myfiles_part = []
    for j in range(100,400): #was 100
        als = np.random.uniform(170,180)
        myname = str(als).replace(".","_")
        myfile.write("cp examples/Pythia8/configNoLHE.cmnd examples/Pythia8/config_"+str(j)+"_"+myname+".cmnd\n")
        myfile.write("sed -i -e 's/6:m0 = 175/6:m0 = "+str(als)+"/g' examples/Pythia8/config_"+str(j)+"_"+myname+".cmnd\n")
        myfile.write("sed -i -e 's/Random:seed = -1/Random:seed = "+str(makeseed(j+500*i))+"/g' examples/Pythia8/config_"+str(j)+"_"+myname+".cmnd\n")
        myfile.write("./DelphesPythia8 cards/delphes_card_CMS.tcl examples/Pythia8/config_"+str(j)+"_"+myname+".cmnd delphes_nolhe"+str(j)+"_"+myname+".root\n")
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
        myfiles_det+=["output_det_"+str(j)+"_"+myname+".txt"]
        myfiles_part+=["output_part_"+str(j)+"_"+myname+".txt"]
        pass
    myfiles_det_str = ""
    myfiles_part_str = ""
    for ii in range(len(myfiles_det)):
        myfiles_det_str+=myfiles_det[ii]+" "
        myfiles_part_str+=myfiles_part[ii]+" "
        pass
    myfile.write("cat "+myfiles_det_str+"> output_det_"+str(i)+".txt\n")
    myfile.write("cat "+myfiles_part_str+"> output_part_"+str(i)+".txt\n")

    for ii in range(len(myfiles_det)):
        myfile.write("rm "+myfiles_det[ii]+"\n")
        myfile.write("rm "+myfiles_part[ii]+"\n")
        pass

    os.system("chmod +x run_"+str(i)+".sh") 
    os.system("bsub -o run_"+str(i)+".log run_"+str(i)+".sh")
