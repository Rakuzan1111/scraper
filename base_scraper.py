import requests
from bs4 import BeautifulSoup
import csv
import re
import logging
from logging.handlers import RotatingFileHandler
import time

handler = RotatingFileHandler(filename = 'logs.txt', maxBytes=1_000_000, backupCount=3)
logging.basicConfig(level=logging.DEBUG, format='%(asctime)s:%(levelname)s:%(message)s', handlers=[handler])
logging.info('New entry: ----------------------------------------------------------------------')



# Headers that makes kijiji recognize requests as human requests
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

# Get initial response to see if website is responding
response = requests.get('https://www.kijiji.ca/b-a-louer/grand-montreal/c30349001l80002', headers=headers)

if response.status_code != 200:
    logging.error(f'Initial request failed: {response.status_code}')
    print('Request failed:', response.status_code)
    exit()

# Listing attributes
fields = {
        'bedrooms': 'Bedrooms',
        'bathrooms': 'Bathrooms',
        'unit_type': 'Unit type',
        'parking': 'Parking included',
        'size': 'Size (sqft)'
        }

# Used to filter duplicate listings by taking the listing website
seen = set()
new = 0
duplicate = 0

# Writes the new csv file
with open("listings.csv", 'w', newline='', encoding='UTF-8-SIG') as f:
    writer = csv.writer(f)
    writer.writerow(['Listing ID', 'Price ($)', 'Description', 'Location', 'Bedrooms', 'Bathrooms', 'Unit Type', 'Parking', 'Size (sqft)', 'Website'])

    base_url = 'https://www.kijiji.ca/b-a-louer/grand-montreal/{page_part}c30349001l80002'

    for page in range(1, 30):
        time.sleep(1)
        # Changes Url depending on page number
        page_part = '' if page == 1 else f'page-{page}/'
        url = base_url.format(page_part=page_part)

        try:
            response = requests.get(url, headers=headers, params={'view': 'list'}, timeout=10)
            response.raise_for_status()

        except requests.exceptions.RequestException as e:
            logging.error(f"Failed to fetch page {page}: {e}")
            continue  

        # Parses html and extracts all "card (section)" objects    
        soup = BeautifulSoup(response.text, 'html.parser')
        cards = soup.find_all('section', attrs={'data-testid': 'listing-card'})
            
        # Gets information from the extracted cards
        
        for information in cards:
            listing_id = None
            try:
                listing = information.find('a', attrs={'data-testid': 'listing-link'})
                website = listing.get('href')
                listing_id = website.split('/')[-1]
                
                # Filters duplicates with set()
                if website in seen:
                    duplicate += 1
                    logging.debug(f'Listing {listing_id} is already in listings. Skip over.')
                    continue

                seen.add(website)
                new += 1

                #Base attributes that all listings have (formatted with re)
                price = information.find('p', attrs={'data-testid': 'listing-price'}).text
                price = re.sub(r'[$,]', '', price)

                description = information.find('a', attrs={'data-testid': 'listing-link'}).text

                location = information.find('div', attrs={'data-testid': 'listing-details'}).text
                
                # More information that can be present on the listing
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

                writer.writerow([listing_id] + list(data.values()) + [website])
                logging.debug(f'Added listing {listing_id} to listings')

            except Exception as e:
                logging.error(f"Error Occured on page {page} with listing {listing_id}: {e}")
                continue


print('Unique listings: ', new) 
print('Duplicated(Removed) listings: ', duplicate)       
