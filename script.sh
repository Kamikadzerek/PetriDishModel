#LANG=en_US
for h in $(LANG=fr_FR seq 1.0 0.01 2.99)
do 
  for i in {0..3}
  do
  #cat run_header.sh >run.sh
    > run.sh
    echo "./PetriDishModel  $h $i $((i+1))" >> run.sh;
    cat run.sh;
#  qsub run.sh;
    #sleep 1
    #echo "-----------------------------------------------------------";
  done;
done;
for h in $(LANG=fr_FR seq 3.0 0.02 3.98)
do 
  for i in {0..3}
  do
  #cat run_header.sh >run.sh
    > run.sh
    echo "./PetriDishModel  $h $i $((i+1))" >> run.sh;
    cat run.sh;
#  qsub run.sh;
    #sleep 1
    #echo "-----------------------------------------------------------";
  done;
done;
for h in $(LANG=fr_FR seq 4.0 0.5 9.5)
do 
  for i in {0..3}
  do
  #cat run_header.sh >run.sh
    > run.sh
    echo "./PetriDishModel  $h $i $((i+1))" >> run.sh;
    cat run.sh;
#  qsub run.sh;
    #sleep 1
    #echo "-----------------------------------------------------------";
  done;
done;
for h in $(LANG=fr_FR seq 10.0 1.0 20)
do 
  for i in {0..3}
  do
  #cat run_header.sh >run.sh
    > run.sh
    echo "./PetriDishModel  $h $i $((i+1))" >> run.sh;
    cat run.sh;
#  qsub run.sh;
    #sleep 1
    #echo "-----------------------------------------------------------";
  done;
done;
