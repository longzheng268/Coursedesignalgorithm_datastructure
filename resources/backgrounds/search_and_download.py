import os
import requests
from bs4 import BeautifulSoup

# 保存目录
save_dir = "/home/LongZheng/Code_Project/Coursedesignalgorithm_datastructure/resources/backgrounds"

# 创建目录（如果不存在）
os.makedirs(save_dir, exist_ok=True)

# 必应图片搜索URL
search_url = "https://www.bing.com/images/search"

# 搜索关键词
search_keyword = "1920x1080 background images"

# 下载图片
def download_image(url, save_path):
    try:
        response = requests.get(url, stream=True)
        response.raise_for_status()
        with open(save_path, 'wb') as file:
            for chunk in response.iter_content(chunk_size=8192):
                file.write(chunk)
        print(f"成功下载: {save_path}")
    except requests.exceptions.RequestException as e:
        print(f"下载失败: {e}")

# 搜索图片并下载
def search_and_download():
    params = {"q": search_keyword, "form": "HDRSC2"}
    response = requests.get(search_url, params=params)
    soup = BeautifulSoup(response.content, "html.parser")
    image_elements = soup.find_all("a", class_="iusc")

    image_urls = []
    for element in image_elements:
        m = element.get("m")
        if m:
            m_dict = eval(m)
            image_urls.append(m_dict.get("murl"))

    for idx, url in enumerate(image_urls[:1000]):  # 限制下载前10张图片
        file_name = f"background_{idx+1}.jpg"
        save_path = os.path.join(save_dir, file_name)
        download_image(url, save_path)

search_and_download()
