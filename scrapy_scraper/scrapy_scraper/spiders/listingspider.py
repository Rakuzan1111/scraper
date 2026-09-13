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

    #For later scrapy versions
    async def start(self):
        for request in self.start_requests():
            yield request

    def start_requests(self):
        
        url = "https://www.kijiji.ca/b-a-louer/grand-montreal/c30349001l80002"
        for page in range(1):
            
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
                listing_website = card.css('h3 a::attr(href)').get()

                if listing_website:
                    listing_id = listing_website.rstrip('/').split('/')[-1]
                    
                else:
                    listing_id = 'N/A'
                    logger.warning('Listing has no website')

                # Filters duplicates with set()
                if listing_website in self.seen:
                    self.duplicate += 1
                    logger.debug(f'Listing {listing_id} is already in listings. Skip over.')
                    continue
                
                self.seen.add(listing_website)
                self.new += 1

                size_text = card.css('li[aria-label="Size (sqft)"] ::text').get(default='N/A')
                size_match = re.search(r'\d+', size_text)

                listing = {
                    'Listing ID' : listing_id,
                    'Price ($)' : re.sub(r'[$,]', '', card.css('p[data-testid="listing-price"]::text').get(default='N/A')),
                    'Description' : card.css('a[data-testid="listing-link"]::text').get(default='N/A'),
                    'Location' : card.css('p[data-testid="listing-location"]::text').get(default='N/A'),
                    'Bedrooms' : card.css('li[aria-label="Bedrooms"] ::text').get(default='N/A'),
                    'Bathrooms' : card.css('li[aria-label="Bathrooms"] ::text').get(default='N/A'),
                    'Unit Type' : card.css('li[aria-label="Unit type"] ::text').get(default='N/A'),
                    'Parking' : card.css('li[aria-label="Parking included"] ::text').get(default='N/A'),
                    'Size (sqft)' : size_match.group() if size_match else 'N/A',
                    
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
    Website: {listing_website}""")
                
            except Exception as e:
                logger.error(f"Error Occured on page {response.url} with listing {listing_id}: {e}")
                continue

            yield response.follow(listing_website, callback=self.parse_listing_page, meta={'listing': listing})

    SECTION_HEADERS = frozenset({
        'Rental agreement', 'Utilities', 'Furnished', 'Appliances',
        'Includes', 'Smoking', 'Accessibility', 'Building amenities',
    })

    def parse_listing_page(self, response):
        listing = response.meta['listing']

        try:
            raw = response.css('div[data-testid="vip-attributes-section"] ::text').getall()
            items = [t.strip() for t in raw if t and t.strip()]
        except Exception as e:
            logger.error(f"Error Occured on page {response.url} with listing {listing['Listing ID']}: {e}")
            items = []

        # Walk the list, grouping each value under the header that precedes it.
        preamble, sections, current = [], {}, None
        for item in items:
            if item in self.SECTION_HEADERS:
                current = item
                sections.setdefault(current, [])
            elif current is None:
                preamble.append(item)
            else:
                sections[current].append(item)

        def first(header, default='N/A'):
            vals = sections.get(header) or []
            return vals[0] if vals else default

        def joined(header):
            vals = sections.get(header) or []
            return ', '.join(vals) if vals else 'N/A'

        def find(pattern, default='N/A'):
            for item in preamble:
                if re.search(pattern, item, re.I):
                    return item
            return default

        # 'Available <date>' / 'Immediate' lives under the Rental agreement header;
        # separate it from the lease term by content, not position.
        avail_re = re.compile(r'^(Available|Immediate)', re.I)
        availability = next((i for i in items if avail_re.match(i)), 'N/A')
        lease = next(
            (v for v in sections.get('Rental agreement', []) if not avail_re.match(v)),
            'N/A',
        )

        utilities = sections.get('Utilities') or []
        def utility(name):
            return next((u for u in utilities if name.lower() in u.lower()), f'{name} Unknown')

        further_listing_information = {
            'Pets Allowed': find(r'\bPets?\b'),
            'Rental agreement': lease,
            'Availability': availability,
            'Utility': f"{utility('Heat')}, {utility('Hydro')}, {utility('Water')}",
            'Furnished': first('Furnished'),
            'Appliances': joined('Appliances'),
            'Includes': joined('Includes'),
            'Building amenities': joined('Building amenities'),
        }

        listing.update(further_listing_information)
        yield listing