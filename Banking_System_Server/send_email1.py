import os.path
import base64
import pickle
import google.auth
import google.auth.transport.requests
from google.oauth2.credentials import Credentials
from google_auth_oauthlib.flow import InstalledAppFlow
from googleapiclient.discovery import build
from googleapiclient.errors import HttpError
from email.mime.text import MIMEText

# If modifying these SCOPES, delete the file token.pickle.
SCOPES = ['https://www.googleapis.com/auth/gmail.send']

def get_credentials():
    creds = None
    if os.path.exists('token.pickle'):
        with open('token.pickle', 'rb') as token:
            creds = pickle.load(token)
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(google.auth.transport.requests.Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(
                '../../../credentials.json', SCOPES)
            creds = flow.run_local_server(port=0)
        with open('token.pickle', 'wb') as token:
            pickle.dump(creds, token)
    return creds

def create_message(sender, to, subject, message_text):
    message = MIMEText(message_text)
    message['to'] = to
    message['from'] = sender
    message['subject'] = subject
    raw = base64.urlsafe_b64encode(message.as_bytes())
    return {'raw': raw.decode()}

def send_message(service, user_id, message):
    try:
        message = (service.users().messages().send(userId=user_id, body=message).execute())
        print('Message Id: %s' % message['id'])
        return message
    except HttpError as error:
        print(f'An error occurred: {error}')
        return None

def main(sender, to, subject, body):
    creds = get_credentials()
    try:
        service = build('gmail', 'v1', credentials=creds)
        message = create_message(sender, to, subject, body)
        send_message(service, 'me', message)
    except HttpError as error:
        print(f'An error occurred: {error}')

if __name__ == '__main__':
    import sys
    if len(sys.argv) != 5:
        print("Usage: python script.py <sender> <recipient> <subject> <body>")
        sys.exit(1)

    sender = sys.argv[1]
    recipient = sys.argv[2]
    subject = sys.argv[3]
    body = sys.argv[4]

    main(sender, recipient, subject, body)
