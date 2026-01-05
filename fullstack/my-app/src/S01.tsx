function S01() {
  const register = async (e: React.FormEvent) => {
    e.preventDefault();

    const formData = new FormData(e.target as HTMLFormElement);
    const data = {
      username: formData.get('username'),
      password: formData.get('password'),
      signUpFor: formData.get('signUpFor'),
      interest: formData.get('interest'),
      spamEmails: formData.get('spamEmails') === 'on',
      bio: formData.get('bio')
    };

    try {
      // const response = await fetch('/api/register', {
      //   method: 'POST',
      //   headers: { 'Content-Type': 'application/json' },
      //   body: JSON.stringify(data)
      // });
      console.log("Register Success", data);
    } catch (error) {
      console.error("Error occurred:", error);
    }
  }

  return (
    <div>
      <h1>Registration Form</h1>
      <form onSubmit={register}>
        <table>
          <tbody>
            <tr>
              <td align="left"><label>Username</label></td>
              <td align="left"><input type="text" name="username" /></td>
            </tr>
            <tr>
              <td align="left"><label>Password</label></td>
              <td align="left"><input type="password" name="password" /></td>
            </tr>
            <tr>
              <td align="left"><label>Sign up for:</label></td>
              <td align="left">
                <select name="signUpFor">
                  <option value="links">Links to New Videos (Free)</option>
                  <option value="news">Super-Valuable Newsletter($29)</option>
                  <option value="course">Online Step-by-Step Course($79)</option>
                  <option value="coach">One-on-One Expert Coaching($149)</option>
                </select>
              </td>
            </tr>
            <tr>
              <td align="left" valign="top"><label>I am <strong>most</strong> interested in:</label></td>
              <td align="left">
                <input type="radio" name="interest" value="Justice" /> Justice
                <br />
                <input type="radio" name="interest" value="Freedom" /> Freedom
                <br />
                <input type="radio" name="interest" value="Equanimity" /> Equanimity
              </td>
            </tr>
            <tr>
              <td align="left"></td>
              <td align="left">
                <input type="checkbox" name="spamEmails" /> Please sign me up for your spam emails
              </td>
            </tr>
            <tr>
              <td align="left" valign="top"><label>Provide an optional short bio:</label></td>
              <td align="left"><textarea rows={4} cols={50} name="bio" /></td>
            </tr>
            <tr>
              <td align="left"></td>
              <td align="left"><button type="submit">Register</button></td>
            </tr>
          </tbody>
        </table>
      </form>
    </div>
  );
}

export default S01;