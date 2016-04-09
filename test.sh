FILES=testScripts/*

for f in $FILES
do
  echo "Processing $f file..."
  cat $f
  compile $f
  echo ""
done
