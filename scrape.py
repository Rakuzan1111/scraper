import requests
from bs4 import BeautifulSoup
import csv
import re
from PIL import Image
from io import BytesIO
import imagehash
import time




headers = {"User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/124.0.0.0 Safari/537.36",
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8",
    "Accept-Language": "en-US,en;q=0.9",
    "Accept-Encoding": "gzip, deflate, br",
    "Connection": "keep-alive",
    "Upgrade-Insecure-Requests": "1",
    "Sec-Fetch-Dest": "document",
    "Sec-Fetch-Mode": "navigate",
    "Sec-Fetch-Site": "none",
    "Sec-Fetch-User": "?1",
    "Sec-Ch-Ua": '"Chromium";v="124", "Google Chrome";v="124", "Not-A.Brand";v="99"',
    "Sec-Ch-Ua-Mobile": "?0",
    "Sec-Ch-Ua-Platform": '"Windows"',}
response = requests.get('https://www.kijiji.ca/b-a-louer/grand-montreal/c30349001l80002', headers=headers)


soup = BeautifulSoup(response.text, 'html.parser')
cards = soup.find_all('div', 'sc-eb45309b-0 kafBMp sc-ed38c1d6-11 gsLzQi')

fields = {
        'bedrooms': 'Bedrooms',
        'bathrooms': 'Bathrooms',
        'unit_type': 'Unit type',
        'parking': 'Parking included',
        'size': 'Size (sqft)'
        }

local_img = Image.open('duProprio.webp')
reference_hash = imagehash.phash(local_img)


with open("listings.csv", 'w', newline='', encoding='UTF-8-SIG') as f:
    writer = csv.writer(f)
    writer.writerow(['Price ($)', 'Description', 'Location', 'Bedrooms', 'Bathrooms', 'Unit Type', 'Parking', 'Size (sqft)', 'Website'])

    for page in range(1, 10):
        params={'view': 'list',
                'page': page}
        
        response = requests.get('https://www.kijiji.ca/b-immobilier/grand-montreal/c34l80002', headers=headers, params=params)
        soup = BeautifulSoup(response.text, 'html.parser')
        cards = soup.find_all('section', attrs={'data-testid': 'listing-card'})
        
        for information in cards:

            img_url = information.find('img', attrs={"data-testid": 'logo-container-non-mobile'})
            if img_url:
                img_url = img_url.get('src')
                img = requests.get(img_url, headers=headers)
                web_img = Image.open(BytesIO(img.content))
                time.sleep(0.1)
                web_hash = imagehash.phash(web_img)
                difference = reference_hash - web_hash

                if difference < 5:
                    continue
            

            price = information.find('p', attrs={'data-testid': 'listing-price'}).text
            price = re.sub(r'[$,]', '', price)

            description = information.find('a', attrs={'data-testid': 'listing-link'}).text

            location = information.find('div', attrs={'data-testid': 'listing-details'}).text
            
            further_information = information.find('ul', attrs={'data-testid': 're-attribute-list-non-mobile'})
                 
            data = {'price': price,
                    'description': description,
                    'location': location
                    }    
                
            if further_information:
                for name, label in fields.items():
                    element = further_information.find('li', attrs={'aria-label': label})
                    data[name] = element.get_text() if element else 'N/A'

            else:
                for name in fields.keys():
                    data[name] = 'N/A'
            if data['size'] != 'N/A':
                data['size'] = re.search(r'\d+', data['size']).group()
            

            website = information.find('a').get('href')
               
            
            

       

       
            
           
  
            writer.writerow(list(data.values()) + [website])
        
