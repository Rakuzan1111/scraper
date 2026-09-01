from pathlib import Path
import pandas as pd
import time
import logging


def check_prices():

    current_listings=pd.read_csv('listings.csv', encoding='utf-8-sig')

    #keep important columns
    current_prices=current_listings[['Listing ID', 'Description', 'Location', 'Price ($)']].copy()

    current_prices["Listing ID"] = current_prices["Listing ID"].astype(str)
    current_prices["Price ($)"] = current_prices["Price ($)"].astype(str)

    history_file=Path('price_history.csv')


    if history_file.exists():
        price_history=pd.read_csv(history_file, encoding='utf-8-sig')
        last_prices=(price_history.sort_values('Checked At')
            .drop_duplicates(subset='Listing ID', keep='last')
            .set_index('Listing ID')['Price ($)'])
        current_prices['Previous Price']=current_prices['Listing ID'].map(last_prices)
        changed_prices=current_prices[current_prices["Previous Price"].isna()
            |(current_prices['Price ($)']!=current_prices['Previous Price'])
        ].drop(columns="Previous Price")
    else:
        price_history=pd.DataFrame()
        changed_prices=current_prices.copy()

    #time when price was checked
    changed_prices['Checked At']=pd.Timestamp.now().strftime('%Y-%m-%d %H:%M:%S')


    #sort lsiting by date
    price_history=pd.concat([price_history, changed_prices], ignore_index=True)

    price_history.to_csv('price_history.csv', index=False, encoding='utf-8-sig')

check_every_second=60*60*24

while True:
    try:
        check_prices()
    except Exception as error:
        logging.exception()
time.sleep(check_every_second)
