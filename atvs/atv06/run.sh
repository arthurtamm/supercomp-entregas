for i in $(seq 9 3 36)
do
    echo "Running with input entrada$i"
    ./main ./inputs/input$i.txt
done
