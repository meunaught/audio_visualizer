echo "started"
youtube-dl -ci -f 'bestaudio' -x --audio-format wav "$1"
echo "finished"