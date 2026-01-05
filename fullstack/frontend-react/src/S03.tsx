import { useState, type JSX } from "react";

const ALLOWED_CHARS = /[a-zA-Z0-9\- .:=;!%]/; // Regex

const getPasswordStrengthComment = (length: number): JSX.Element => {
  if (length < 6) {
    return <p>Minimum length is 6 characters</p>;
  } else if (length < 10) {
    return <p>Weak Password &mdash; longer passwords are more secure</p>;
  }
  return <p>Strong Password</p>;
};

export default function S03() {
  const [password, setPassword] = useState("");
  const [comment, setComment] = useState<JSX.Element | null>(null);

  const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    // Note that setting state is asynchronous, so we capture the new value in a variable
    const newPassword = e.target.value
      .split("")
      .filter((x) => x.match(ALLOWED_CHARS))
      .join("");
    setPassword(newPassword);
    setComment(getPasswordStrengthComment(newPassword.length));
  };

  const handleClick = (e: React.MouseEvent<HTMLButtonElement>) => {
    e.preventDefault();
    console.log("Registered with password: ", password);
  };

  return (
    <div>
      <h1>Registration Form</h1>
      <form>
        Password&nbsp;
        <input type="password" value={password} onChange={handleChange} />
        {comment}
        <button onClick={handleClick}>Register</button>
      </form>
    </div>
  );
}