import pandas as pd
import csv 

df = pd.read_csv("listings.csv", encoding="utf-8-sig")

# convert n/a to string
for column in df.columns:
    df[column] = df[column].fillna("N/A").astype(str)
    
#remove duplicate websites
df = df.drop_duplicates(subset="Website")

# Make numeric temporary columns for correct numerical sorting
df["price_sort"] = pd.to_numeric(df["Price ($)"].str.replace(r"[^\d.]", "", regex=True),
    errors="coerce")

df["bedrooms_sort"] = pd.to_numeric(df["Bedrooms"].str.extract(r"(\d+)")[0],
    errors="coerce")


df["bathrooms_sort"] = pd.to_numeric(df["Bathrooms"].str.extract(r"(\d+(?:\.\d+)?)")[0],
    errors="coerce")

df['parking_sort'] = pd.to_numeric(df['Parking'].str.extract(r'(\d+)')[0],
    errors='coerce')

df["size_sort"] = pd.to_numeric(df["Size (sqft)"].str.extract(r"(\d+)")[0],
    errors="coerce")

# make list to remove temporary columns
helper_columns=["price_sort","bedrooms_sort","bathrooms_sort","size_sort",'parking_sort']

#title of each table
tables=[('listing sorted by price', 'price_sort'),
        ('listing sorted by location', 'Location'),
        ('listing sorted by bedrooms', 'bedrooms_sort'),
        ('listing sorted by bathrooms', 'bathrooms_sort'),
        ('listing sorted by unit type', 'Unit Type'),
        ('listing sorted by parking spaces', 'parking_sort'),
        ('listing sorted by size', 'size_sort')]

#write every table into one csv file
with open("listing_sorted.csv", 'w', newline='', encoding='utf-8-sig') as file:
    writer=csv.writer(file)

    for title, sort_column in tables:
        writer.writerow([title])
        writer.writerow([])
        sorted_table=df.sort_values(sort_column, na_position='last').drop(columns=helper_columns)
        sorted_table.to_csv(file, index=False)
        writer.writerow([])
        writer.writerow([])
        
