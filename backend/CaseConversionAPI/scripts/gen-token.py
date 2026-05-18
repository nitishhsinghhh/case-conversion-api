import jwt
import datetime

# EXACT same strings from your appsettings.json
secret = "SuperSecretKey_NitishSingh_2026_HighPerformance"
issuer = "CaseConversion-Gateway"
audience = "CaseConversion-Client"

payload = {
    "iss": issuer,
    "aud": audience,
    "sub": "NitishSingh",
    "exp": datetime.datetime.utcnow() + datetime.timedelta(days=1)
}

token = jwt.encode(payload, secret, algorithm="HS256")
print(f"Bearer {token}")