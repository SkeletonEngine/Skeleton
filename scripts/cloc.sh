echo "Counting lines of code in the skeleton project..."
git ls-files skeleton/src | xargs wc -l

echo "Counting lines of code in the skeleton-editor project..."
git ls-files skeleton-editor/src | xargs wc -l

echo "Counting lines of code in the skeleton-runtime project..."
git ls-files skeleton-runtime/src | xargs wc -l
