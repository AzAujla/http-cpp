find src -name "*.cpp" -o -name "*.h" | sort | xargs -I{} sh -c 'echo "=== {} ===" && cat {}'
