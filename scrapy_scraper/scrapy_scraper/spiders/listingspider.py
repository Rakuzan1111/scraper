import scrapy
import re
import logging
from logging.handlers import RotatingFileHandler
import time

handler = RotatingFileHandler(filename='logs.txt', maxBytes=1_000_000, backupCount=3, encoding='UTF-8-SIG')
formatter = logging.Formatter('%(asctime)s:%(levelname)s:%(message)s\n')
handler.setFormatter(formatter)

logger = logging.getLogger('listingslogger')
logger.setLevel(logging.DEBUG)
logger.addHandler(handler)
logger.propagate = False

logger.info('New entry: ----------------------------------------------------------------------')





class ListingspiderSpider(scrapy.Spider):
    name = "listingspider"
    allowed_domains = ["kijiji.ca"]

    # Used to filter duplicate listings by taking the listing website
    seen = set()
    new = 0
    duplicate = 0

    def start_requests(self):
        
        url = "https://www.kijiji.ca/b-a-louer/grand-montreal/c30349001l80002"
        for page in range(1, 50):
            
            if page != 1:
                url = f"https://www.kijiji.ca/b-a-louer/grand-montreal/page-{page}/c30349001l80002"
            yield scrapy.Request(
                url=url,
                callback=self.parse
            )

    

    def parse(self, response):
        print("STATUS:", response.status)
        print("URL:", response.url)
         
        cards = response.css('section[data-testid="listing-card"]')
        
        if not cards:
            logger.warning(f"No listing cards found | status={response.status} | url={response.url}")
            return

        print("CARDS:", len(cards))

        for card in cards:
            listing_id = 'N/A'
            try:
                website = card.css('h3 a::attr(href)').get()

                if website:
                    listing_id = website.rstrip('/').split('/')[-1]
                else:
                    listing_id = 'N/A'
                    logger.warning('Listing has no website')

                # Filters duplicates with set()
                if website in self.seen:
                    self.duplicate += 1
                    logger.debug(f'Listing {listing_id} is already in listings. Skip over.')
                    continue
                
                self.seen.add(website)
                self.new += 1

                listing = {
                    'Listing ID' : listing_id,
                    'Price ($)' : card.css('p[data-testid="listing-price"]::text').get(default='N/A').strip('$'),
                    'Description' : card.css('a[data-testid="listing-link"]::text').get(default='N/A'),
                    'Location' : card.css('p[data-testid="listing-location"]::text').get(default='N/A'),
                    'Bedrooms' : card.css('li[aria-label="Bedrooms"] ::text').get(default='N/A'),
                    'Bathrooms' : card.css('li[aria-label="Bathrooms"] ::text').get(default='N/A'),
                    'Unit Type' : card.css('li[aria-label="Unit type"] ::text').get(default='N/A'),
                    'Parking' : card.css('li[aria-label="Parking included"] ::text').get(default='N/A'),
                    'Size (sqft)' : card.css('li[aria-label="Size (sqft)"] ::text').get(default='N/A'),
                    'Website' :  website
                    }
                logger.debug(f"""Added listing {listing_id} to listings with properties
    Listing ID: {listing_id} 
    Price : ${listing['Price ($)']} 
    Description: {listing['Description']} 
    Location: {listing['Location']} 
    Bedrooms: {listing['Bedrooms']} 
    Bathrooms: {listing['Bathrooms']}
    Unit Type: {listing['Unit Type']}
    Parking: {listing['Parking']}
    Size (sqft): {listing['Size (sqft)']} 
    Website: {website}""")
                
            except Exception as e:
                logger.error(f"Error Occured on page {response.url} with listing {listing_id}: {e}")
                continue

            yield listing

        
        
            

