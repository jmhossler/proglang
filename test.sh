FILES=testScripts/*

for f in $FILES
do
  echo "Processing $f file..."
  cat $f
  echo ""
  compile $f
  echo ""
done
