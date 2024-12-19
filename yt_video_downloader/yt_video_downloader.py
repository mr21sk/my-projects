import yt_dlp

def download_youtube_video(video_url):
    try:
        # Set download options to avoid FFmpeg
        ydl_opts = {
            'format': 'best',  # Downloads the best combined audio and video stream
            'outtmpl': '%(title)s.%(ext)s',  # Save with the video title as filename
        }

        # Download the video
        with yt_dlp.YoutubeDL(ydl_opts) as ydl:
            info_dict = ydl.extract_info(video_url, download=True)
            print(f"Download completed: {info_dict['title']}")
    except Exception as e:
        print("An error occurred:", e)

# Input the YouTube video URL
video_url = input("Enter the YouTube video URL: ")
download_youtube_video(video_url)




