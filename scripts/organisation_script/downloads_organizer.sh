#!/bin/bash

echo "Organizing folder..."

DOWNLOAD_DIR="/mnt/c/Users/faki/Downloads"

cd "$DOWNLOAD_DIR" || exit 1

declare -A categories

categories=(
[Images]="jpg jpeg png gif bmp svg webp"
[Videos]="mp4 mkv mob avi webp"
[Music]="mp3 wav flac ogg"
[Documents]="pdf doc docx xls xlsx ppt pptx txt md"
[Archives]="zip tar gz rar 7z iso"
[Programs]="exe msi deb rpm dmg"
)

for folder in "${!categories[@]}" Others; do 
	mkdir -p "$folder"
done

move_file() {
	local file="$1"
	local ext="${file##*.}"
	ext="${ext,,}"

	for category in "${!categories[@]}"; do
		if [[ "${categories[$category]}" =~ $ext ]]; then
			mv -n "$file" "$category/" && echo "Moved: $file-> $category/"
			return
		fi
	done
	mv -n "$file" "Others/" && echo "Moved: $file->Others/"
}

shopt -s nullglob
for f in *; do
	[[ -f "$f" ]] && move_file "$f"
done

shopt -u nullglob

find "$DOWNLOAD_DIR" -type d -empty -delete

echo "Folder organised!"

