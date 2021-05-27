echo "Started:"
youtube-dl -o "wav/%(title)s.%(ext)s" -ci -f 'bestaudio' -x --audio-format wav "$1"
echo "Finished!"