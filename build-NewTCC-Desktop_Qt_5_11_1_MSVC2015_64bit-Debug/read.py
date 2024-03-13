
import sys
from google.auth.transport.requests import Request
from google.oauth2.credentials import Credentials
from google_auth_oauthlib.flow import InstalledAppFlow
from googleapiclient.discovery import build
from googleapiclient.errors import HttpError

# If modifying these scopes, delete the file token.json.
SCOPES = ['https://www.googleapis.com/auth/spreadsheets']

def get_row(spreadsheet_id, range_name):
  """
  Creates the batch_update the user has access to.
  Load pre-authorized user credentials from the environment.
  TODO(developer) - See https://developers.google.com/identity
  for guides on implementing OAuth2 for the application.
  """
  creds = Credentials.from_authorized_user_file("token.json", SCOPES)
  # pylint: disable=maybe-no-member
  try:
    service = build("sheets", "v4", credentials=creds)

    result = (
        service.spreadsheets()
        .values()
        .get(spreadsheetId=spreadsheet_id, range=range_name)
        .execute()
    )
    rows = result.get("values", [])

    res_list = []

    for item in rows: 
        if item and item not in res_list: 
            res_list.append(item[0])

    return res_list
  
  except HttpError as error:
    return None

def get_values(spreadsheet_id, range_name):
  # print(range_name)
  """
  Creates the batch_update the user has access to.
  Load pre-authorized user credentials from the environment.
  TODO(developer) - See https://developers.google.com/identity
  for guides on implementing OAuth2 for the application.
  """
  creds = Credentials.from_authorized_user_file("token.json", SCOPES)
  # pylint: disable=maybe-no-member
  try:
    service = build("sheets", "v4", credentials=creds)

    result = (
        service.spreadsheets()
        .values()
        .get(spreadsheetId=spreadsheet_id, range=range_name)
        .execute()
    )
    rows = result.get("values", [])

    return rows
  
  except HttpError as error:
    return None

if __name__ == "__main__":
    SAMPLE_SPREADSHEET_ID = open('spreadsheet_id', 'r').readline().strip()

    testCaseIDs = get_row(SAMPLE_SPREADSHEET_ID, sys.argv[1]+"!A2:A")
    
    index = testCaseIDs.index(sys.argv[2])

    row  = get_values(SAMPLE_SPREADSHEET_ID, sys.argv[1]+"!A" + str(index+2) + ":I" + str(index+2))
    print(f"{row}")